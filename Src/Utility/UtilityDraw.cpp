#include "UtilityDraw.h"
#include "Utility3D.h"
#include "UtilityCommon.h"


void UtilityDraw::DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len)
{
    auto nDir = Utility3D::VNormalize(dir);
    auto sPos = VAdd(pos, VScale(nDir, -len));
    auto ePos = VAdd(pos, VScale(nDir, len));
    DrawLine3D(sPos, ePos, color);
    DrawSphere3D(ePos, 5.0f, 5, color, color, true);
}

void UtilityDraw::DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len)
{

    VECTOR dir;

    // X
    dir = VTransform(Utility3D::DIR_R, rot);
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = VTransform(Utility3D::DIR_U, rot);
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = VTransform(Utility3D::DIR_F, rot);
    DrawLineDir(pos, dir, 0x0000ff, len);

}

void UtilityDraw::DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len)
{

    VECTOR dir;

    // X
    dir = rot.GetRight();
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = rot.GetUp();
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = rot.GetForward();
    DrawLineDir(pos, dir, 0x0000ff, len);

}

void UtilityDraw::DrawPointLine3D(const VECTOR sPos, const VECTOR ePos, int color, float len)
{
    VECTOR dir = VNorm(VSub(ePos, sPos));
    VECTOR currentPos = sPos;
    while (true)
    {
        DrawLine3D(currentPos, VAdd(currentPos, VScale(dir, len)), color);
        currentPos = VAdd(currentPos, VScale(dir, len * 2.0f));
        if (Utility3D::Magnitude(VSub(currentPos, ePos)) < len)
        {
            break;
        }
    }
}

void UtilityDraw::DrawStringCenter(const int _centerX, const int _centerY, const std::wstring _str, unsigned int color, const int fontHandle)
{
    int w, h;
    GetDrawStringSizeToHandle(
        &w,
        &h,
        NULL,
        _str.c_str(),
		static_cast<int>(wcslen(_str.c_str())),
        fontHandle);
    int x = _centerX - (w / 2);
    int y = _centerY - (h / 2);
	DrawStringToHandle(x, y, _str.c_str(), color, fontHandle);
}
