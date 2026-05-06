#include "Utility3D.h"
#include <cmath>
#include "Utility2D.h"

VECTOR Utility3D::RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad)
{
    float x = ((radiusPos.x - centerPos.x) * cosf(rad)) - ((radiusPos.z - centerPos.z) * sinf(rad));
    float z = ((radiusPos.x - centerPos.x) * sinf(rad)) + ((radiusPos.z - centerPos.z) * cosf(rad));
    return VGet(centerPos.x + x, radiusPos.y, centerPos.z + z);
}

double Utility3D::Magnitude(const VECTOR& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Utility3D::MagnitudeF(const VECTOR& v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Utility3D::SqrMagnitudeF(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double Utility3D::SqrMagnitude(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double Utility3D::SqrMagnitude(const VECTOR& v1, const VECTOR& v2)
{
    return pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2);
}

double Utility3D::Distance(const VECTOR& v1, const VECTOR& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}

bool Utility3D::IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
    // 球体同士の衝突判定
    bool ret = false;

    // お互いの半径の合計
    float radius = radius1 + radius2;

    // 座標の差からお互いの距離を取る
    VECTOR diff = VSub(pos2, pos1);

    // 三平方の定理で比較(SqrMagnitudeと同じ)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

bool Utility3D::IsHitSphereCapsule(
    const VECTOR& sphPos, float sphRadius,
    const VECTOR& capPos1, const VECTOR& capPos2, float capRadius)
{

    bool ret = false;

    // カプセル球体の中心を繋ぐベクトル
    VECTOR cap1to2 = VSub(capPos2, capPos1);

    // ベクトルを正規化
    VECTOR cap1to2ENor = VNorm(cap1to2);

    // カプセル繋ぎの単位ベクトルと、
    // そのベクトル元から球体へのベクトルの内積を取る
    float dot = VDot(cap1to2ENor, VSub(sphPos, capPos1));

    // 内積で求めた射影距離を使って、カプセル繋ぎ上の座標を取る
    VECTOR capRidePos = VAdd(capPos1, VScale(cap1to2ENor, dot));

    // カプセル繋ぎのベクトルの長さを取る
    float len = Utility3D::MagnitudeF(cap1to2);

    // 球体がカプセル繋ぎ上にいるか判別するため、比率を取る
    float rate = dot / len;

    VECTOR centerPos;

    // 球体の位置が３エリアに分割されたカプセル形状のどこにいるか判別
    if (rate > 0.0f && rate <= 1.0f)
    {
        // 球体がカプセル繋ぎ上にいる
        centerPos = VAdd(capPos1, VScale(cap1to2ENor, dot));
    }
    else if (rate > 1.0f)
    {
        // 球体がカプセルの終点側にいる
        centerPos = capPos2;
    }
    else if (rate < 0.0f)
    {
        // 球体がカプセルの始点側にいる
        centerPos = capPos1;
    }
    else
    {
        // ここにきてはいけない
    }

    // 球体同士の当たり判定
    if (Utility3D::IsHitSpheres(centerPos, capRadius, sphPos, sphRadius))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}



bool Utility3D::Equals(const VECTOR& v1, const VECTOR& v2)
{
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

bool Utility3D::EqualsVZero(const VECTOR& v1)
{
    const VECTOR& v2 = VECTOR_ZERO;
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

VECTOR Utility3D::Normalize(const Vector2& v)
{
    VECTOR ret = VGet(
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        0.0f
    );
    float len = static_cast<float>(Utility2D::Magnitude(v));
    ret.x /= len;
    ret.y /= len;
    ret.z /= len;
    return ret;
}

VECTOR Utility3D::VNormalize(const VECTOR& v)
{
    if (Utility3D::EqualsVZero(v))
    {
        // Quaternion計算でゼロを渡して、
        // エラー(-1, -1, -1)が返ってくると困る
        return v;
    }
    return VNorm(v);
}

double Utility3D::AngleDeg(const VECTOR& from, const VECTOR& to)
{
    auto fLen = SqrMagnitude(from);
    auto tLen = SqrMagnitude(to);
    auto denominator = sqrt(fLen * tLen);
    if (denominator < kEpsilonNormalSqrt)
    {
        return 0.0f;
    }

    auto dot = VDot(from, to) / denominator;
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    if (dot > 1.0f)
    {
        dot = 1.0f;
    }

    return acos(dot) * (180.0 / DX_PI);
}

const VECTOR Utility3D::GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed)
{
    //標的への方向ベクトルを取得
    VECTOR targetVec = VSub(_goal, _start);

    if (Utility3D::EqualsVZero(targetVec))
    {
        return targetVec;
    }

    //正規化
    targetVec = VNorm(targetVec);

    //移動量を求める
    VECTOR ret = VScale(targetVec, _speed);

    return ret;
}

const VECTOR Utility3D::GetRotAxisToTarget(const VECTOR _pos, const VECTOR _targetPos, const VECTOR _needAxis)
{
    //対象までの方向ベクトル
    VECTOR targetVec = GetMoveVec(_pos, _targetPos);

    //回転量
    Quaternion turQuaRot = Quaternion::LookRotation(targetVec);

    //VECTOR変換
    VECTOR ret = turQuaRot.ToEuler();

    //必要ない軸は除去
    ret.x *= _needAxis.x;
    ret.y *= _needAxis.y;
    ret.z *= _needAxis.z;

    return ret;
}

const VECTOR Utility3D::AddPosRotate(const VECTOR& _followPos, const Quaternion& _followRot, const VECTOR& _localPos)
{
    //座標回転
    VECTOR addPos = _followRot.PosAxis(_localPos);

    VECTOR ret = VAdd(_followPos, addPos);

    //足したものを返す
    return ret;
}

bool Utility3D::IsBeyondGoalPos(const VECTOR _startPos, const VECTOR _goalPos, const VECTOR _mydir)
{
    return false;
}

const VECTOR Utility3D::ReverseValue(const VECTOR _vec)
{
    return VScale(_vec, RESERVE_NUM);
}




