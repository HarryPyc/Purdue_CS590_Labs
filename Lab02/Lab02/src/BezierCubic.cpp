#include "BezierCubic.h"
#include <iostream>


BezierCubic::BezierCubic()
{
}

BezierCubic::BezierCubic(vec3 p0, vec3 p1, vec3 p2, vec3 p3, int n)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->n = n;
	InitBezierCubic();
}


BezierCubic::~BezierCubic()
{
}

vec3 BezierCubic::GetBezierPoint(float t)
{
	//De Casteljau representation
	vec3 p0_1 = (1 - t) * p0 + t*p1,
		 p1_1 = (1 - t) * p1 + t*p2,
		 p2_1 = (1 - t) * p2 + t*p3;
	vec3 p0_2 = (1 - t) * p0_1 + t*p1_1,
	 	 p1_2 = (1 - t) * p1_1 + t*p2_1;
	return (1 - t)*p0_2 + t*p1_2;
}

vec3 BezierCubic::GetBezier(float t)
{
	return p0 * B0(t) + p1 * B1(t) + p2 * B2(t) + p3 * B3(t);
}

vec3 BezierCubic::GetD1Bezier(float t) {
	float B0_1 = -3 * (1 - t)*(1 - t);
	float B1_1 = -6 * t*(1 - t) + 3 * (1 - t)*(1 - t);
	float B2_1 = -3 * t*t + 6 * t*(1 - t);
	float B3_1 = 3 * t*t;
	return p0 * B0_1 + p1 * B1_1 + p2 * B2_1 + p3 * B3_1;
}

vec3 BezierCubic::GetD2Bezier(float t) {
	float B0_2 = -6 * (t - 1);
	float B1_2 = 18 * t - 12;
	float B2_2 = 6 - 18 * t;
	float B3_2 = 6 * t;
	return p0 * B0_2 + p1 * B1_2 + p2 * B2_2 + p3 * B3_2;
}

void BezierCubic::InitBezierCubic() {
	b.clear();
	for (int i = 0; i < n; i++) {
		b.push_back(GetBezierPoint(float(i) / (n - 1)));
	}
}

BezierCubic C1BezierCubic(BezierCubic *b1, vec3 p2, vec3 p3, int n) {
	vec3 p0 = b1->p3;
	vec3 p1 = p0 + p0 - b1->p2;
	return BezierCubic(p0, p1, p2, p3, n);
}

BezierCubic G1BezierCubic(BezierCubic *b1, vec3 p2, vec3 p3, int n, float k) {
	vec3 p0 = b1->p3;
	vec3 p1 = p0 + (p0 - b1->p2) * k;
	return BezierCubic(p0, p1, p2, p3, n);
}

BezierCubic Line(vec3 start, vec3 end, int s)
{
	BezierCubic b(start, start, end, end, 2);
	b.start = s;
	b.end = s + 1;
	return b;
}



inline float ChordLengthParameterization(vec3 p, vec3 start, vec3 end) {
	float t = length(p - start) / (length(p - start)+ length(end - p));
	return t;
}

Error GetError(vector<vec3> *points, BezierCubic *bezier, vector<float> *u) {
    float max_error = 0;
	Error err;
	const int N = points->size();
	for (int i = 0; i < N; i++) {
		vec3 p = points->at(i);
		float e = length2(p - bezier->GetBezier(u->at(i)));
		if (e > max_error) {
			max_error = e;
			err.index = i;
			err.error = e;
		}
	}
	return err;
}


BezierCubic FitCubic(vector<vec3> *points, vec3 t1, vec3 t2, vector<float> *u) {
	const int N = points->size();
	vec3 startPoint = points->at(0);
	vec3 endPoint = points->at(N - 1);

	float a1 = 0, a2 = 0;
	BezierCubic b(startPoint, startPoint + a1 * t1, endPoint + a2 * t2, endPoint, N);

	//compute c11, c12, c21, c22 and X1, X2
	float c11 = 0.f, c12 = 0.f, c21 = 0.f, c22 = 0.f, X1 = 0.f, X2 = 0.f;

	for (int i = 0; i < N; i++) {
		vec3 Ai_1 = t1 * b.B1(u->at(i));
		vec3 Ai_2 = t2 * b.B2(u->at(i));
		c11 += dot(Ai_1, Ai_1);
		c12 += dot(Ai_1, Ai_2);
		c21 += dot(Ai_1, Ai_2);
		c22 += dot(Ai_2, Ai_2);
		vec3 temp = points->at(i) - (b.p0 * b.B0(u->at(i)) + b.p0 * b.B1(u->at(i)) +
			b.p3 * b.B2(u->at(i)) + b.p3 * b.B3(u->at(i)));
		X1 += dot(temp, Ai_1);
		X2 += dot(temp, Ai_2);
	}
	//cout << "c11:" << c11 << endl
	//	<< "c12:" << c12 << endl
	//	<< "c21:" << c21 << endl
	//	<< "c22:" << c22 << endl
	//	<< "X1:" << X1 << endl
	//	<< "X2:" << X2 << endl;
	float C[4] = {
		c11,c12,
		c21,c22
	};
	mat2 C1C2 = make_mat2(C);
	float xc2[4] = {
		X1,c12,
		X2,c22
	};
	mat2 XC2 = make_mat2(xc2);
	float c1x[4] = {
		c11,X1,
		c21,X2
	};
	mat2 C1X = make_mat2(c1x);
	//Cramer's Rule
	a1 = determinant(XC2) / determinant(C1C2);
	a2 = determinant(C1X) / determinant(C1C2);

	b.p1 = b.p0 + a1 * t1;
	b.p2 = b.p3 + a2 * t2;
	b.InitBezierCubic();
	return b;
}
void NewtonIteration(vector<vec3> *points, vector<float> *u, BezierCubic *b) {
	for (int i = 0; i < u->size(); i++) {
		float t = u->at(i);
		vec3 Q = b->GetBezier(t), Q1 = b->GetD1Bezier(t), Q2 = b->GetD2Bezier(t);
		t = t - dot(Q - points->at(i), Q1) / (dot(Q2, Q - points->at(i)) +
			dot(Q1, Q1));
		u->at(i) = t;
	}
}
void FitCurve(vector<vec3> *points, vector<BezierCubic> *beziers, vector<Error> *errors, int start) {
	const int N = points->size();
	cout << "segments size =" << N << endl;	
	vec3 startPoint = points->at(0);
	vec3 endPoint = points->at(N - 1);
	if (N == 2) {
		beziers->push_back(Line(startPoint, endPoint, start));
		return;
	}

	int window = N / 10 + 2;
	//get normalized vector p0_p1(t1) and p3_p2(t2)
	vec3 t1 = vec3(0.0), t2 = vec3(0.0);
	for (int i = 1; i < window; i++) {
		t1 += points->at(i) - startPoint;
		t2 += points->at(N - i - 1) - endPoint;
	}

	t1 = normalize(t1);
	t2 = normalize(t2);
	vector<float> u;
	for (int i = 0; i < N; i++) {
		u.push_back(ChordLengthParameterization(points->at(i), startPoint, endPoint));
	}
	BezierCubic bezier = FitCubic(points, t1, t2, &u);
	for (int i = 0; i < max_iteration; i++) {
		NewtonIteration(points, &u, &bezier);
		bezier = FitCubic(points, t1, t2, &u);
	}
	Error err = GetError(points, &bezier, &u);
	//cout << err.error << endl;
	bezier.start = start;
	bezier.end = start + N - 1;
	err.index += start;
	errors->push_back(err);
	beziers->push_back(bezier);
}
bool compError(const Error &a, const Error &b) {
	return a.error > b.error;
}
bool compBezier(const BezierCubic &a, const BezierCubic &b) {
	return a.start < b.start;
}
bool hasApproximated = false;
int lastNum = 0;
void BezierApproximation(vector<vec3> *points, vector<BezierCubic> *beziers, int *num)
{
	if (hasApproximated && *num > lastNum)
	{
		*num = lastNum;
		return;
	}
	lastNum = *num;
	int count = *num;
	const int N = points->size();

	vector<Error> errors;
	beziers->clear();
	FitCurve(points, beziers, &errors, 0);

	
	while (count-- > 1) {
		int n = beziers->size();
		sort(errors.begin(), errors.end(), compError);
		Error maxError = errors[0];
		cout << "Max Error locates in" << maxError.index <<' '<<maxError.error<< endl;
		errors.erase(errors.begin());
		for (int i = 0; i < n; i++) {
			if (beziers->at(i).start <= maxError.index &&
				beziers->at(i).end >= maxError.index) {
				vector<vec3> p1, p2;
				int breakIndex = maxError.index;
				//assign doesn't include second argument
				p1.assign(points->begin() + beziers->at(i).start, points->begin() + breakIndex + 1);
				p2.assign(points->begin() + breakIndex, points->begin() + beziers->at(i).end + 1);

	
				FitCurve(&p1, beziers, &errors, beziers->at(i).start);		
				FitCurve(&p2, beziers, &errors, breakIndex);
				beziers->erase(beziers->begin() + i);
				break;
			}
		}
	}

	float sum_error = 0.f;
	for (int i = 0; i < errors.size(); i++) {
		sum_error += errors[i].error;
	}
	cout << "Total Error =" << sum_error << endl;
	if (sum_error < MAX_ERROR)
		hasApproximated = true;
	else
		hasApproximated = false;
	sort(beziers->begin(), beziers->end(), compBezier);
	for (int i = 0; i < beziers->size(); i++) {
		cout << "Start and End Index : " << beziers->at(i).start
			<< ' ' << beziers->at(i).end << endl
			<< "length = " << beziers->at(i).n << endl;
	}

	
}



