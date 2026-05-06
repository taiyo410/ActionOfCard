#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "Easing.h"


Easing::Easing(void)
{
    easingFuncTable_ = {
        {EASING_TYPE::LERP, [this](float t) {easingUpdate_ = [this, t](float) {return Lerp(t); }; }},
        {EASING_TYPE::LERP_COMEBACK, [this](float t) {easingUpdate_ = [this, t](float) {return LerpBack(t); }; }},
		{EASING_TYPE::OUT_BACK, [this](float t) {easingUpdate_ = [this, t](float) {return OutBack(t); }; }},
        {EASING_TYPE::QUAD_IN, [this](float t) {easingUpdate_ = [this, t](float) {return EaseQuadIn(t); }; }},
        {EASING_TYPE::QUAD_OUT, [this](float t) {easingUpdate_ = [this, t](float) {return EaseQuadOut(t); }; }},
        {EASING_TYPE::QUAD_IN_OUT, [this](float t) {easingUpdate_ = [this, t](float) {return EaseQuadInOut(t); }; }},
        {EASING_TYPE::QUAD_OUT_IN, [this](float t) {easingUpdate_ = [this, t](float) {return EaseQuadOutIn(t); }; }},
        {EASING_TYPE::QUAD_BACK, [this](float t) {easingUpdate_ = [this, t](float) {return EaseQuadBack(t); }; }},
        {EASING_TYPE::CUBIC_IN, [this](float t) {easingUpdate_ = [this, t](float) {return EaseCubicIn(t); }; }},
        {EASING_TYPE::CUBIC_OUT, [this](float t) {easingUpdate_ = [this, t](float) {return EaseCubicOut(t); }; }},
        {EASING_TYPE::EXPO, [this](float t) {easingUpdate_ = [this, t](float) {return EaseExpo(t); }; }},
        {EASING_TYPE::SIN_BACK, [this](float t) {easingUpdate_ = [this, t](float) {return EaseSinBack(t); }; }},
        {EASING_TYPE::COS_BACK, [this](float t) {easingUpdate_ = [this, t](float) {return EaseCosBack(t); }; }},
        {EASING_TYPE::ELASTIC_IN, [this](float t) {easingUpdate_ = [this, t](float) {return EaseInElastic(t); }; }},
        {EASING_TYPE::ELASTIC_OUT, [this](float t) {easingUpdate_ = [this, t](float) {return EaseOutElastic(t); }; }},
		{EASING_TYPE::ELASTIC_BACK, [this](float t) {easingUpdate_ = [this, t](float) {return EaseBackElastic(t); }; }},
		{EASING_TYPE::BOUNCE, [this](float t) {easingUpdate_ = [this, t](float) {return EaseBounce(t); }; }}
    };

}

Easing::~Easing(void)
{
}

void Easing::SetEasing(const float t, const EASING_TYPE type)
{
    easingFuncTable_[type](t);
}

int Easing::EaseFunc(const int start, const int end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    int dis = end - start;
    int ret = UtilityCommon::Round(start + dis * easingUpdate_(t));
    return ret;
}

float Easing::EaseFunc(const float start, const float end, const float t,const EASING_TYPE type)
{
    SetEasing(t, type);
    float dis = end - start;
    return start + dis * easingUpdate_(t);
}
double Easing::EaseFunc(const double start, const double end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    double dis = end - start;
    return start + dis * easingUpdate_(t);
}
Vector2F Easing::EaseFunc(const Vector2F& start, const Vector2F& end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    Vector2F dis = end - start;
    return start + dis * easingUpdate_(t);
}
Vector2 Easing::EaseFunc(const Vector2& start, const Vector2& end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    Vector2 dis = end - start;
    Vector2 ret = {};
    ret.x = UtilityCommon::Round(start.x + dis.x * easingUpdate_(t));
    ret.y = UtilityCommon::Round(start.y + dis.y * easingUpdate_(t));
    return ret;
}
VECTOR Easing::EaseFunc(const VECTOR& start, const VECTOR& end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    VECTOR dis = VSub(end, start);
    VECTOR power = VScale(dis, easingUpdate_(t));
    return VAdd(start, power);
}

COLOR_F Easing::EaseFunc(const COLOR_F& start, const COLOR_F& end, const float t, const EASING_TYPE type)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }
    SetEasing(t, type);
    COLOR_F ret = start;
    ret.r += (end.r - start.r) * easingUpdate_(t);
    ret.g += (end.g - start.g) * easingUpdate_(t);
    ret.b += (end.b - start.b) * easingUpdate_(t);
    ret.a += (end.a - start.a) * easingUpdate_(t);
    return ret;
}



float Easing::EaseFuncDeg(float& start, float& end, const float t, const EASING_TYPE type)
{
    float ret;

    float diff = end - start;
    if (diff < -HALF_DEG_MAX)
    {
        end += DEG_MAX;
        ret = EaseFunc(start, end, t, type);
        if (ret >= DEG_MAX)
        {
            ret -= DEG_MAX;
        }
    }
    else if (diff > HALF_DEG_MAX)
    {
        end -= DEG_MAX;
        ret = EaseFunc(start, end, t, type);
        if (ret < 0.0)
        {
            ret += DEG_MAX;
        }
    }
    else
    {
        ret = EaseFunc(start, end, t, type);
    }

    return ret;
}

double Easing::EaseFuncDeg(double& start, double& end, float t, const EASING_TYPE type)
{
    double ret;

    double diff = end - start;
    if (diff < -HALF_DEG_MAX)
    {
        end += DEG_MAX;
        ret = EaseFunc(start, end, t, type);
        if (ret >= DEG_MAX)
        {
            ret -= DEG_MAX;
        }
    }
    else if (diff > HALF_DEG_MAX)
    {
        end -= DEG_MAX;
        ret = EaseFunc(start, end, t, type);
        if (ret < 0.0)
        {
            ret += DEG_MAX;
        }
    }
    else
    {
        ret = EaseFunc(start, end, t, type);
    }

    return ret;
}

float Easing::EaseFuncRad(float& start, float& end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    float diff = remainder(end - start, 2.0f * DX_PI_F);
    return start + diff * easingUpdate_(t);
}


float Easing::Lerp(const float t)
{
    if (t > EASING_MAX)return EASING_MAX;
    //割合だけを返して、Funcで計算
    return t;
}

float Easing::LerpBack(const float t)
{
    if (t > EASING_MAX)return 0.0f;
    float ret = 0.0f;
    if (t <= EASING_HALF)
    {
        ret = Lerp(t / EASING_HALF);
    }
    else
    {
        ret = Lerp((1.0f - t) / EASING_HALF);
    }
    return ret;
}

float Easing::OutBack(const float t)
{
	if (t > EASING_MAX)return EASING_MAX;
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;

    return 1.0f + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
}



float Easing::EaseOutElastic(const float t)
{
    if (t > EASING_MAX)return EASING_MAX;
    const float ELASTIC_DECAY = 10.0f;  //減衰率
    const float ELASTIC_FREQUENCY = 3.0f; //振動数
    const float ANGULAR_FREQ = DX_TWO_PI_F / ELASTIC_FREQUENCY;       //サイン波がどれだけの速さで変化するか
    const float PHASE_SHIFT = 0.75f; //位相のずれ
    const float OFFSET = 1.0f; //オフセット

    float ret = 0.0f;
    ret = (powf(2.0f, -ELASTIC_DECAY * t) * sinf((t * ELASTIC_DECAY - PHASE_SHIFT) * ANGULAR_FREQ) + OFFSET);
    return ret;
}

float Easing::EaseBackElastic(const float t)
{
    if (t > EASING_MAX)return 0.0f;
    float ret = 0.0f;
    const float c4 = (DX_TWO_PI_F / 3.0f) - 6.1f;

    //この式はグラフを見ながら試行錯誤した式
    ret = (powf(2, -10.0f * t) * sinf((t * 10.0f - 0.088f) * c4));
    return ret;
}


float Easing::EaseBounce(const float t)
{
    if (t > EASING_MAX)return EASING_MAX;
    const float BOUNCE_POW = 2.75f;
    const float BOUNCE_ACCEL = 7.5625;
    MATH_FUNC quad;
    float ret = 0.0f;
    quad.accel = BOUNCE_ACCEL;
    if (t < EASING_MAX / BOUNCE_POW)
    {
        return EaseQuadIn(t / (EASING_MAX / BOUNCE_POW));
    }
    else if (1.0f / BOUNCE_POW <= t && t < 2.0f / BOUNCE_POW)
    {
        quad.graph_vertex = { 1.5f / BOUNCE_POW,0.75f };
        ret = quad.QuadFunc(t);

    }
    else if (2.0f / BOUNCE_POW <= t && t < 2.5f / BOUNCE_POW)
    {
        quad.graph_vertex = { 2.25f / BOUNCE_POW,0.9375f };
        ret = quad.QuadFunc(t);
    }
    else
    {
        quad.graph_vertex = { 2.625f / BOUNCE_POW,0.984375f };
        ret = quad.QuadFunc(t);
    }
    return ret;
}


float Easing::EaseQuadIn(const float t)
{
    if (t >= EASING_MAX)return EASING_MAX;
    MATH_FUNC quad;
    float ret = quad.QuadFunc(t);
    return ret;
}



float Easing::EaseQuadOut(const float t)
{
    if (t >= EASING_MAX)return EASING_MAX;
    MATH_FUNC quad;
    quad.accel = -EASING_MAX;
    quad.graph_vertex = { EASING_MAX,EASING_MAX };
    float ret = quad.QuadFunc(t);
    return ret;
}

float Easing::EaseQuadBack(const float t)
{
    if (t > EASING_MAX)return 0.0f;
    MATH_FUNC ret;
    ret.accel = -4.0f;
    ret.graph_vertex = { EASING_HALF,EASING_MAX };

    return ret.QuadFunc(t);
}


float Easing::EaseQuadInOut(const float t)
{
    if (t >= EASING_MAX)return EASING_MAX;
    float ret = 0.0f;
    MATH_FUNC quad;
    if (t <= HALF)
    {
        quad.accel = 2.0f;
    }
    else
    {
        quad.graph_vertex = { EASING_MAX,EASING_MAX };
        quad.accel = -2.0f;
    }
    ret = quad.QuadFunc(t);
    return ret;
}


float Easing::EaseQuadOutIn(const float t)
{
    if (t >= EASING_MAX)return EASING_MAX;
    float ret = 0.0f;
    MATH_FUNC quad;
    if (t < EASING_HALF)
    {
        quad.accel = -2.0f;
        quad.graph_vertex = { EASING_HALF,EASING_HALF };
    }
    else
    {
        quad.accel = 2.0f;
        quad.graph_vertex = { EASING_HALF,EASING_HALF };
    }
    ret = quad.QuadFunc(t);
    return ret;
}


float Easing::EaseCubicIn(const float t)
{
    if (t >= EASING_MAX)return EASING_MAX;
    MATH_FUNC qubic;
    float ret = qubic.CubicFunc(t);
    return ret;
}


float Easing::EaseCubicOut(const float t)
{
    if (t >= EASING_MAX)return EASING_MAX;
    MATH_FUNC qubic;
    qubic.graph_vertex = { -EASING_MAX,EASING_MAX };
    float ret = qubic.CubicFunc(t);
    return ret;
}

float Easing::EaseExpo(const float t, const int expo)
{
    if (t >= EASING_MAX)return EASING_MAX;
    float ret = 0.0f;
    float scaled = powf(EASING_MAX - t, static_cast<float>(expo));
    float base = EASING_MAX - scaled;
    float inv_t = EASING_MAX / t;
    float expoFunc = powf(base, inv_t);
    ret = expoFunc;

    return ret;
}

float Easing::EaseInElastic(const float t)
{
    if (t >= EASING_MAX)return EASING_MAX;
    float ret = 0.0f;
    const float c4 = (2.0f * DX_PI_F) / 3.0f;
    ret = -(powf(2, 10.0f * t - 10) * sinf((t * 10.0f - 10.75f) * c4) + 1.0f);
    return ret;
}


float Easing::EaseSinBack(const float t)
{
    if (t >= EASING_MAX)return 0.0f;
    TRIG_FUNC sinFunc;
    sinFunc.amplitude = 1.0f;
    sinFunc.lambda = 2.0f;
    return sinFunc.SinFunc(t);;
}

float Easing::EaseCosBack(const float t)
{
    if (t >= EASING_MAX)return 0.0f;
    TRIG_FUNC cosFunc;
    cosFunc.amplitude = -EASING_HALF;
    cosFunc.lambda = EASING_MAX;
    return cosFunc.CosFunc(t);
}

Vector2F Easing::EaseEpiCycloid(const Vector2F& start, const float t, const float halfRadiusNum, const float smallRadius)
{
    if (t > EASING_MAX)return start;

    float rad = DX_TWO_PI_F * t;
    float baseRadius = smallRadius * halfRadiusNum;
    Vector2F ret = {};
    ret.x = (baseRadius + smallRadius) * cos(rad) - smallRadius * cos(((baseRadius + smallRadius) / smallRadius) * rad);
    ret.y = (baseRadius + smallRadius) * sin(rad) - smallRadius * sin(((baseRadius + smallRadius) / smallRadius) * rad);
    return start + ret;
}

Vector2F Easing::EaseHypoCycloid(const Vector2F& start, const float t, const float halfRadiusNum, const float smallRadius)
{
    if (t > EASING_MAX)return start;

    float rad = DX_TWO_PI_F * t;
    float baseRadius = smallRadius * halfRadiusNum;
    Vector2F ret = {};
    ret.x = (baseRadius - smallRadius) * cos(rad) + smallRadius * cos(((baseRadius - smallRadius) / smallRadius) * rad);
    ret.y = (baseRadius - smallRadius) * sin(rad) - smallRadius * sin(((baseRadius - smallRadius) / smallRadius) * rad);
    return start + ret;
}