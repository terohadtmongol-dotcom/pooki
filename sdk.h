#include <corecrt_math_defines.h>
#include <random>
#include <d3d9types.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <minwinbase.h>
#include <string.h>
#include <list>
#include <iostream>
#include <processthreadsapi.h>
#include <WinUser.h>
#include "settings.h"

class Vector2
{
public:
    Vector2() : x(0.f), y(0.f) {}
    Vector2(double _x, double _y) : x(_x), y(_y) {}
    ~Vector2() {}
    double x, y;
};

class Vector3
{
public:
    Vector3() : x(0.f), y(0.f), z(0.f) {}
    Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
    ~Vector3() {}

    double x, y, z;

    inline double dot(Vector3 v) { return x * v.x + y * v.y + z * v.z; }

    inline double distance(Vector3 v) {
        return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
    }

    Vector3 operator-(Vector3 v) {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    float Length() const {
        return sqrtf(static_cast<float>(x * x + y * y + z * z));
    }
};

class fvector2d
{
public:
    fvector2d() : x(0.f), y(0.f)
    {

    }

    fvector2d(double _x, double _y) : x(_x), y(_y)
    {

    }
    ~fvector2d()
    {

    }

    fvector2d operator-(fvector2d v)
    {
        return fvector2d(x - v.x, y - v.y);
    }

    double x;
    double y;
};

class fvector
{
public:
    fvector() : x(0.f), y(0.f), z(0.f)
    {

    }

    fvector(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
    {

    }
    ~fvector()
    {

    }

    double x;
    double y;
    double z;

    inline double dot(fvector v)
    {
        return x * v.x + y * v.y + z * v.z;
    }

    inline double distance(fvector v)
    {
        return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
    }

    inline double length() {
        return sqrt(x * x + y * y + z * z);
    }

    fvector operator+(fvector v)
    {
        return fvector(x + v.x, y + v.y, z + v.z);
    }

    fvector operator-(fvector v)
    {
        return fvector(x - v.x, y - v.y, z - v.z);
    }

    fvector operator/(double flNum)
    {
        return fvector(x / flNum, y / flNum, z / flNum);
    }

    fvector operator*(double flNum) { return fvector(x * flNum, y * flNum, z * flNum); }
};
struct fquat
{
    double x;
    double y;
    double z;
    double w;
};
struct frotator
{
    double Pitch;
    double Yaw;
    double Roll;
};

struct FTransform
{
    fquat rotation;
    fvector translation;
    uint8_t pad1c[0x8];
    fvector scale3d;
    uint8_t pad2c[0x8];

    D3DMATRIX to_matrix_with_scale()
    {
        D3DMATRIX m{};

        const fvector Scale
        (
            (scale3d.x == 0.0) ? 1.0 : scale3d.x,
            (scale3d.y == 0.0) ? 1.0 : scale3d.y,
            (scale3d.z == 0.0) ? 1.0 : scale3d.z
        );

        const double x2 = rotation.x + rotation.x;
        const double y2 = rotation.y + rotation.y;
        const double z2 = rotation.z + rotation.z;
        const double xx2 = rotation.x * x2;
        const double yy2 = rotation.y * y2;
        const double zz2 = rotation.z * z2;
        const double yz2 = rotation.y * z2;
        const double wx2 = rotation.w * x2;
        const double xy2 = rotation.x * y2;
        const double wz2 = rotation.w * z2;
        const double xz2 = rotation.x * z2;
        const double wy2 = rotation.w * y2;

        m._41 = translation.x;
        m._42 = translation.y;
        m._43 = translation.z;
        m._11 = (1.0f - (yy2 + zz2)) * Scale.x;
        m._22 = (1.0f - (xx2 + zz2)) * Scale.y;
        m._33 = (1.0f - (xx2 + yy2)) * Scale.z;
        m._32 = (yz2 - wx2) * Scale.z;
        m._23 = (yz2 + wx2) * Scale.y;
        m._21 = (xy2 - wz2) * Scale.y;
        m._12 = (xy2 + wz2) * Scale.x;
        m._31 = (xz2 + wy2) * Scale.z;
        m._13 = (xz2 - wy2) * Scale.x;
        m._14 = 0.0f;
        m._24 = 0.0f;
        m._34 = 0.0f;
        m._44 = 1.0f;

        return m;
    }
};
FTransform* transform;
D3DMATRIX to_matrix(fvector rot, fvector origin = fvector(0, 0, 0))
{
    float radpitch = (rot.x * M_PI / 180);
    float radyaw = (rot.y * M_PI / 180);
    float radroll = (rot.z * M_PI / 180);
    float sp = sinf(radpitch);
    float cp = cosf(radpitch);
    float sy = sinf(radyaw);
    float cy = cosf(radyaw);
    float sr = sinf(radroll);
    float cr = cosf(radroll);
    D3DMATRIX matrix{};
    matrix.m[0][0] = cp * cy;
    matrix.m[0][1] = cp * sy;
    matrix.m[0][2] = sp;
    matrix.m[0][3] = 0.f;
    matrix.m[1][0] = sr * sp * cy - cr * sy;
    matrix.m[1][1] = sr * sp * sy + cr * cy;
    matrix.m[1][2] = -sr * cp;
    matrix.m[1][3] = 0.f;
    matrix.m[2][0] = -(cr * sp * cy + sr * sy);
    matrix.m[2][1] = cy * sr - cr * sp * sy;
    matrix.m[2][2] = cr * cp;
    matrix.m[2][3] = 0.f;
    matrix.m[3][0] = origin.x;
    matrix.m[3][1] = origin.y;
    matrix.m[3][2] = origin.z;
    matrix.m[3][3] = 1.f;
    return matrix;
}

inline D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
    D3DMATRIX pOut;
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

    return pOut;
}
#define PI 3.14159265358979323846f
struct _MATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

        };
        float m[4][4];
    };
};
inline _MATRIX Matrix(fvector Vec4, fvector origin = fvector(0, 0, 0))
{
    double radPitch = (Vec4.x * double(PI) / 180.f);
    double radYaw = (Vec4.y * double(PI) / 180.f);
    double radRoll = (Vec4.z * double(PI) / 180.f);

    double SP = sinf(radPitch);
    double CP = cosf(radPitch);
    double SY = sinf(radYaw);
    double CY = cosf(radYaw);
    double SR = sinf(radRoll);
    double CR = cosf(radRoll);

    _MATRIX matrix;
    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.f;

    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.f;

    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.f;

    matrix.m[3][0] = origin.x;
    matrix.m[3][1] = origin.y;
    matrix.m[3][2] = origin.z;
    matrix.m[3][3] = 1.f;

    return matrix;
}

static float powf_(float _X, float _Y)
{
    return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
}

static float sqrtf_(float _X) {
    return (_mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ss(_X))));
}

static double get_cross_distance(double x1, double y1, double x2, double y2) {
    return sqrtf(powf((x2 - x1), 2) + powf_((y2 - y1), 2));
}



#define FNAMEPOOL_OFFSET 0x117284C0

struct WorldData {
    uintptr_t game_state;
};
extern WorldData* world_data_ptr;

D3DMATRIX matrix_multiplication(D3DMATRIX pm1, D3DMATRIX pm2)
{
    D3DMATRIX pout{};
    pout._11 = pm1._11 * pm2._11 + pm1._12 * pm2._21 + pm1._13 * pm2._31 + pm1._14 * pm2._41;
    pout._12 = pm1._11 * pm2._12 + pm1._12 * pm2._22 + pm1._13 * pm2._32 + pm1._14 * pm2._42;
    pout._13 = pm1._11 * pm2._13 + pm1._12 * pm2._23 + pm1._13 * pm2._33 + pm1._14 * pm2._43;
    pout._14 = pm1._11 * pm2._14 + pm1._12 * pm2._24 + pm1._13 * pm2._34 + pm1._14 * pm2._44;
    pout._21 = pm1._21 * pm2._11 + pm1._22 * pm2._21 + pm1._23 * pm2._31 + pm1._24 * pm2._41;
    pout._22 = pm1._21 * pm2._12 + pm1._22 * pm2._22 + pm1._23 * pm2._32 + pm1._24 * pm2._42;
    pout._23 = pm1._21 * pm2._13 + pm1._22 * pm2._23 + pm1._23 * pm2._33 + pm1._24 * pm2._43;
    pout._24 = pm1._21 * pm2._14 + pm1._22 * pm2._24 + pm1._23 * pm2._34 + pm1._24 * pm2._44;
    pout._31 = pm1._31 * pm2._11 + pm1._32 * pm2._21 + pm1._33 * pm2._31 + pm1._34 * pm2._41;
    pout._32 = pm1._31 * pm2._12 + pm1._32 * pm2._22 + pm1._33 * pm2._32 + pm1._34 * pm2._42;
    pout._33 = pm1._31 * pm2._13 + pm1._32 * pm2._23 + pm1._33 * pm2._33 + pm1._34 * pm2._43;
    pout._34 = pm1._31 * pm2._14 + pm1._32 * pm2._24 + pm1._33 * pm2._34 + pm1._34 * pm2._44;
    pout._41 = pm1._41 * pm2._11 + pm1._42 * pm2._21 + pm1._43 * pm2._31 + pm1._44 * pm2._41;
    pout._42 = pm1._41 * pm2._12 + pm1._42 * pm2._22 + pm1._43 * pm2._32 + pm1._44 * pm2._42;
    pout._43 = pm1._41 * pm2._13 + pm1._42 * pm2._23 + pm1._43 * pm2._33 + pm1._44 * pm2._43;
    pout._44 = pm1._41 * pm2._14 + pm1._42 * pm2._24 + pm1._43 * pm2._34 + pm1._44 * pm2._44;
    return pout;
}
fvector prediction(fvector TargetPosition, fvector ComponentVelocity, float player_distance, float projectile_speed = 239, float projectile_gravity = 3.5)
{
    float TimeToTarget = player_distance / projectile_speed;
    float bulletDrop = abs(projectile_gravity) * (TimeToTarget * TimeToTarget) * 0.5;

    return fvector
    {
        TargetPosition.x + TimeToTarget * ComponentVelocity.x,
        TargetPosition.y + TimeToTarget * ComponentVelocity.y,
        TargetPosition.z + TimeToTarget * ComponentVelocity.z + bulletDrop
    };
}

struct camera_position_s {
    fvector location{};
    fvector rotation{};
    float fov{};
};
inline camera_position_s camera_postion{};

#define deg2Rad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define rad2Deg(angleRadians) ((angleRadians) * 180.0 / M_PI)

struct FMinimalViewInfo
{
    FMinimalViewInfo() : Location(), Rotation(), FOV(FOV) {}
    FMinimalViewInfo(fvector Location, fvector Rotation, float FOV) : Location(Location), Rotation(Rotation), FOV(FOV) {}

    fvector Location, Rotation;
    float FOV;
};

inline FMinimalViewInfo m_camera_information{};


typedef struct _D3DMATRIX1 {
    union {
        struct {
            double _11, _12, _13, _14;
            double _21, _22, _23, _24;
            double _31, _32, _33, _34;
            double _41, _42, _43, _44;

        };
        double m[4][4];
    };
} D3DMATRIX1;

typedef struct D3DXMATRIX1 {
    DOUBLE _ij;
} D3DXMATRIX1, * LPD3DXMATRIX1;

struct FPlane : public fvector
{
    double W;
};

class FMatrix
{
public:
    double m[4][4];
    FPlane XPlane, YPlane, ZPlane, WPlane;

    FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() {}
    FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane)
        : XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) {
    }
};

uintptr_t viewStates;
uintptr_t viewMatrices;
D3DMATRIX1 invProjMatrix;

class Camera
{
public:
    void normalize(fvector& in)
    {
        while (in.x > 180.0f)
            in.x -= 360.0f;

        while (in.x < -180.0f)
            in.x += 360.0f;

        while (in.y > 180.0f)
            in.y -= 360.0f;

        while (in.y < -180.0f)
            in.y += 360.0f;

        in.z = 0;
    }

    auto calc_angle(fvector LocalPos, fvector WorldPos) -> fvector {
        fvector RelativePos = WorldPos - LocalPos;
        float yaw = atan2(RelativePos.y, RelativePos.x) * 180.0f / M_PI;
        float pitch = -((acos(RelativePos.z / LocalPos.distance(WorldPos)) * 180.0f / M_PI) - 90.0f);

        return fvector(pitch, yaw, 0);
    }

}; inline Camera* camera;

std::string string_To_UTF8(const std::string& str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];
    ZeroMemory(pwBuf, nwLen * 2 + 2);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr(pBuf);

    delete[]pwBuf;
    delete[]pBuf;

    pwBuf = NULL;
    pBuf = NULL;

    return retStr;
}

template<typename T>
inline T rotl(T x, int n) noexcept {
    constexpr int bits = sizeof(T) * 8;
    n &= bits - 1;
    return (x << n) | (x >> (bits - n));
}

uintptr_t decryptWorld(uintptr_t address)
{
    return address ^ 0xFFFFFFFF30B9BBF9;
}

//inline uintptr_t decryptWorld(uintptr_t encryptedAddress) noexcept {
//    return encryptedAddress = rotl(encryptedAddress, 55) - 0xFFFFFFFFDC445031uLL;
//}

inline std::wstring MBytesToWString(const char* lpcszString)
{
    int len = strlen(lpcszString);
    int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
    wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
    memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
    ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
    std::wstring wString = (wchar_t*)pUnicode;
    delete[] pUnicode;
    return wString;
}
inline std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
    char* pElementText;
    int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
    pElementText = new char[iTextLen + 1];
    memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
    ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
    std::string strReturn(pElementText);
    delete[] pElementText;
    return strReturn;
}

float arrowsize = 8;
class decryption_t
{
public:

    void AddPlayerToFovCircle(fvector WorldLocation, float fDistance, bool visible, ImColor color)
    {
        fvector vAngle = camera_postion.rotation;
        float fYaw = vAngle.y * PI / 180.0f;
        float dx = WorldLocation.x - camera_postion.location.x;
        float dy = WorldLocation.y - camera_postion.location.y;
        float fsin_yaw = sinf(fYaw);
        float fminus_cos_yaw = -cosf(fYaw);

        auto Center = ImVec2(globals.ScreenWidth / 2, globals.ScreenHeight / 2);

        float x = -(dy * fminus_cos_yaw + dx * fsin_yaw);
        float y = dx * fminus_cos_yaw - dy * fsin_yaw;

        float fovRadius = aimbot.FovSize * 1.5f;
        float angle = atan2f(y, x);
        float triangleSize = arrowsize;
        float widthFactor = 8.0f;
        float outlineThickness = 2.0f;

        float arrowDistance = fovRadius + triangleSize + 5.0f;

        ImVec2 triangleCenter = ImVec2(
            Center.x + cosf(angle) * arrowDistance,
            Center.y + sinf(angle) * arrowDistance
        );
        ImVec2 point1 = ImVec2(
            triangleCenter.x + cosf(angle) * triangleSize,
            triangleCenter.y + sinf(angle) * triangleSize
        );
        ImVec2 point2 = ImVec2(
            triangleCenter.x + cosf(angle + widthFactor) * triangleSize,
            triangleCenter.y + sinf(angle + widthFactor) * triangleSize
        );
        ImVec2 point3 = ImVec2(
            triangleCenter.x + cosf(angle - widthFactor) * triangleSize,
            triangleCenter.y + sinf(angle - widthFactor) * triangleSize
        );

        ImU32 fillColor = ImGui::ColorConvertFloat4ToU32(visible ? color : ImColor(color.Value.x, color.Value.y, color.Value.z, 0.5f));
        ImU32 outlineColor = ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 1.0));

        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        drawList->AddTriangleFilled(point1, point2, point3, fillColor);
        drawList->AddTriangle(point1, point2, point3, outlineColor, outlineThickness);
    }

    std::string getRank(int tier) {
        switch (tier) {
        case 0:  return "Unranked";
        case 1:  return "Bronze 2";
        case 2:  return "Bronze 3";
        case 3:  return "Silver 1";
        case 4:  return "Silver 2";
        case 5:  return "Silver 3";
        case 6:  return "Gold 1";
        case 7:  return "Gold 2";
        case 8:  return "Gold 3";
        case 9:  return "Platinum 1";
        case 10: return "Platinum 2";
        case 11: return "Platinum 3";
        case 12: return "Diamond 1";
        case 13: return "Diamond 2";
        case 14: return "Diamond 3";
        case 15: return "Elite";
        case 16: return "Champion";
        case 17: return "Unreal";
        default: return "Unranked";
        }
    }

    ImVec4 getRankColor(int tier) {
        switch (tier) {
        case 0:  return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // Unranked
        case 1:
        case 2:  return ImVec4(0.902f, 0.580f, 0.227f, 1.0f); // Bronze
        case 3:
        case 4:
        case 5:  return ImVec4(0.843f, 0.843f, 0.843f, 1.0f); // Silver
        case 6:
        case 7:
        case 8:  return ImVec4(1.0f, 0.871f, 0.0f, 1.0f); // Gold
        case 9:
        case 10:
        case 11: return ImVec4(0.0f, 0.7f, 0.7f, 1.0f);  // Platinum
        case 12:
        case 13:
        case 14: return ImVec4(0.1686f, 0.3294f, 0.8235f, 1.0f); // Diamond
        case 15: return ImVec4(0.7f, 0.7f, 0.7f, 1.0f);   // Elite
        case 16: return ImVec4(1.0f, 0.6f, 0.0f, 1.0f);   // Champion
        case 17: return ImVec4(0.6f, 0.0f, 0.6f, 1.0f);   // Unreal
        default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // Unranked
        }
    }
    void radar_range(float* x, float* y, float range)
    {
        if (fabs((*x)) > range || fabs((*y)) > range)
        {
            if ((*y) > (*x))
            {
                if ((*y) > -(*x))
                {
                    (*x) = range * (*x) / (*y);
                    (*y) = range;
                }
                else
                {
                    (*y) = -range * (*y) / (*x);
                    (*x) = -range;
                }
            }
            else
            {
                if ((*y) > -(*x))
                {
                    (*y) = range * (*y) / (*x);
                    (*x) = range;
                }
                else
                {
                    (*x) = -range * (*x) / (*y);
                    (*y) = -range;
                }
            }
        }
    }
    void project_world_to_radar(fvector vOrigin, int& screenx, int& screeny, ImVec2 pos)
    {
        fvector vAngle = camera_postion.rotation;
        auto fYaw = vAngle.y * M_PI / 180.0f;
        float dx = vOrigin.x - camera_postion.location.x;
        float dy = vOrigin.y - camera_postion.location.y;
        float fsin_yaw = sinf(fYaw);
        float fminus_cos_yaw = -cosf(fYaw);
        float x = dy * fminus_cos_yaw + dx * fsin_yaw;
        x = -x;
        float y = dx * fminus_cos_yaw - dy * fsin_yaw;
        float range = (float)(radar.range / 5) * 1000.f;

        radar_range(&x, &y, range);

        ImVec2 DrawPos = ImVec2(pos.x, pos.y);
        ImVec2 DrawSize = ImVec2(200.0f, 200.0f);
        float radarRadius = 200.0f / 2;
        float distance = sqrt(pow(screenx - DrawPos.x, 2) + pow(screeny - DrawPos.y, 2));

        if (distance > radarRadius)
        {
            float angle = atan2(screeny - DrawPos.y, screenx - DrawPos.x);
            screenx = static_cast<int>(DrawPos.x + radarRadius * cos(angle));
            screeny = static_cast<int>(DrawPos.y + radarRadius * sin(angle));
        }

        int rad_x = (int)DrawPos.x;
        int rad_y = (int)DrawPos.y;
        float r_siz_x = DrawSize.x;
        float r_siz_y = DrawSize.y;
        int x_max = (int)r_siz_x + rad_x - 5;
        int y_max = (int)r_siz_y + rad_y - 5;

        screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
        screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

        if (screenx > x_max)
            screenx = x_max;

        if (screenx < rad_x)
            screenx = rad_x;

        if (screeny > y_max)
            screeny = y_max;

        if (screeny < rad_y)
            screeny = rad_y;
    }


    void CalcRadarPoint(fvector vOrigin, int& screenx, int& screeny)
    {
        fvector vAngle = camera_postion.rotation;
        auto fYaw = vAngle.y * M_PI / 180.0f;
        float dx = vOrigin.x - camera_postion.location.x;
        float dy = vOrigin.y - camera_postion.location.y;

        float fsin_yaw = sinf(fYaw);
        float fminus_cos_yaw = -cosf(fYaw);

        float x = dy * fminus_cos_yaw + dx * fsin_yaw;
        x = -x;
        float y = dx * fminus_cos_yaw - dy * fsin_yaw;

        float range = (float)radar.range * 1000.f * radar.scale;

        radar_range(&x, &y, range);

        ImVec2 DrawPos = ImVec2(radar.pos_x, radar.pos_y);
        ImVec2 DrawSize = ImVec2(radar.size, radar.size);


        int rad_x = (int)DrawPos.x;
        int rad_y = (int)DrawPos.y;

        float r_siz_x = DrawSize.x;
        float r_siz_y = DrawSize.y;

        int x_max = (int)r_siz_x + rad_x - 5;
        int y_max = (int)r_siz_y + rad_y - 5;

        screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
        screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

        if (screenx > x_max)
            screenx = x_max;

        if (screenx < rad_x)
            screenx = rad_x;

        if (screeny > y_max)
            screeny = y_max;

        if (screeny < rad_y)
            screeny = rad_y;
    }

    static std::string GetPlayerName(uintptr_t playerState) {
        return get_player_name(playerState);
    }

    static std::string get_player_name(uintptr_t player_state) {
        __int64 FString = kernel->read_t<__int64>(player_state + 0xA00);

        int Length = kernel->read_t<int>(FString + 16);

        uintptr_t FText = kernel->read_t<uintptr_t>(FString + 8);

        __int64 v6 = Length;

        if (!v6) return std::string("BOT");

        wchar_t* NameBuffer = new wchar_t[Length];

        if (!kernel->read(FText, NameBuffer, Length * sizeof(wchar_t))) {
            delete[] NameBuffer;
            return "BOT";
        }

        char v21;
        int v22;
        int i;
        int v25;
        WORD* v23;

        v21 = v6 - 1;

        if (!(DWORD)v6)
            v21 = 0;

        v22 = 0;
        v23 = (WORD*)NameBuffer;

        for (i = (v21) & 3; ; *v23++ += i & 7)
        {
            v25 = v6 - 1;

            if (!(DWORD)v6)
                v25 = 0;

            if (v22 >= v25)
                break;

            i += 3;
            ++v22;
        }

        std::wstring username{ NameBuffer };

        delete[] NameBuffer;

        return std::string(username.begin(), username.end());
    }

};
decryption_t decryption;
struct UObject {
};
#define current_address reinterpret_cast<uint64_t>(this)

class gamehelper_t
{
public:

    static auto getsocketlocation(uintptr_t skeletal_mesh, int bone_index) -> fvector {
        uintptr_t bone_array = kernel->read_t<uintptr_t>(skeletal_mesh + BONE_ARRAY);
        if (bone_array == NULL) bone_array = kernel->read_t<uintptr_t>(skeletal_mesh + BONE_ARRAY + 0x10);

        FTransform bone = kernel->read_t<FTransform>(bone_array + (bone_index * 0x60));
        FTransform component_to_world = kernel->read_t<FTransform>(skeletal_mesh + COMPONENT_TO_WORLD);
        D3DMATRIX matrix = matrix_multiplication(bone.to_matrix_with_scale(), component_to_world.to_matrix_with_scale());

        return fvector(matrix._41, matrix._42, matrix._43);
    }
    fvector target_prediction(fvector TargetPosition, fvector ComponentVelocity, float player_distance, float ProjectileSpeed)
    {
        float gravity = abs(-336);
        float time = player_distance / abs(ProjectileSpeed);
        float bulletDrop = (gravity / 250) * time * time;
        return fvector(TargetPosition.x += time * (ComponentVelocity.x), TargetPosition.y += time * (ComponentVelocity.y), TargetPosition.z += time * (ComponentVelocity.z));
    }
    inline double RadiansToDegrees(double dRadians)
    {
        return dRadians * (180.0 / M_PI);
    }
    uintptr_t get_view_state()
    {
        TArray<uintptr_t> view_state = kernel->read_t<TArray<uintptr_t>>(CachePointers.LocalPlayer + 0xD0);

        return view_state.Get(1);
    }
    camera_position_s get_camera()
    {
        camera_position_s view_point{};

        auto mProjection = kernel->read_t<FMatrix>(get_view_state() + 0x940);
        view_point.rotation.x = RadiansToDegrees(std::asin(mProjection.ZPlane.W));
        view_point.rotation.y = RadiansToDegrees(std::atan2(mProjection.YPlane.W, mProjection.XPlane.W));
        view_point.rotation.z = 0.0;

        view_point.location.x = mProjection.m[3][0];
        view_point.location.y = mProjection.m[3][1];
        view_point.location.z = mProjection.m[3][2];
        float FieldOfView = atanf(1 / kernel->read_t<double>(get_view_state() + 0x740)) * 2;
        view_point.fov = (FieldOfView) * (180.f / M_PI);

        return view_point;


    }
    inline fvector2d ProjectWorldToScreen(fvector WorldLocation)
    {
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);
        int screen_center_x = width / 2;
        int screen_center_y = height / 2;
        camera_postion = get_camera();
        D3DMATRIX temp_matrix = to_matrix(camera_postion.rotation);
        fvector vaxisx = fvector(temp_matrix.m[0][0], temp_matrix.m[0][1], temp_matrix.m[0][2]);
        fvector vaxisy = fvector(temp_matrix.m[1][0], temp_matrix.m[1][1], temp_matrix.m[1][2]);
        fvector vaxisz = fvector(temp_matrix.m[2][0], temp_matrix.m[2][1], temp_matrix.m[2][2]);
        fvector vdelta = WorldLocation - camera_postion.location;
        fvector vtransformed = fvector(vdelta.dot(vaxisy), vdelta.dot(vaxisz), vdelta.dot(vaxisx));

        if (vtransformed.z < 1) vtransformed.z = 1;

        return fvector2d(screen_center_x + vtransformed.x * ((screen_center_x / tanf(camera_postion.fov * (float)M_PI / 360.0f))) / vtransformed.z, screen_center_y - vtransformed.y * ((screen_center_x / tanf(camera_postion.fov * (float)M_PI / 360.0f))) / vtransformed.z);
    }
    fvector PredictHeadshot(fvector TargetPosition, fvector ComponentVelocity, fvector ComponentAcceleration, float player_distance, float ProjectileSpeed = 375.0f, float gravity = 9.8f, float Latency = 0.05f, float HeadOffset = 70.0f)
    {

        float TimeToTarget = player_distance / ProjectileSpeed;

        TimeToTarget += Latency;
        float bulletDrop = gravity * (TimeToTarget * TimeToTarget) * 0.5f;

        TargetPosition.z += HeadOffset;

        fvector PredictedPosition;
        PredictedPosition.x = TargetPosition.x + TimeToTarget * ComponentVelocity.x + 0.5f * ComponentAcceleration.x * (TimeToTarget * TimeToTarget);
        PredictedPosition.y = TargetPosition.y + TimeToTarget * ComponentVelocity.y + 0.5f * ComponentAcceleration.y * (TimeToTarget * TimeToTarget);
        PredictedPosition.z = TargetPosition.z + TimeToTarget * ComponentVelocity.z + 0.5f * ComponentAcceleration.z * (TimeToTarget * TimeToTarget) - bulletDrop;


        PredictedPosition.z += 15.0f * TimeToTarget;

        return PredictedPosition;
    }
    fvector PredictLocation(fvector target, fvector targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
    {
        float gravity = abs(-336);
        float time = distance / abs(projectileSpeed);
        float bulletDrop = (gravity / 250) * time * time;
        return fvector(target.x += time * (targetVelocity.x), target.y += time * (targetVelocity.y), target.z += time * (targetVelocity.z));
    }
    fvector prediction(fvector TargetPosition, fvector ComponentVelocity, float player_distance, float projectile_speed = 239, float projectile_gravity = 3.5)
    {
        float TimeToTarget = player_distance / projectile_speed;
        float bulletDrop = abs(projectile_gravity) * (TimeToTarget * TimeToTarget) * 0.5;

        return fvector
        {
            TargetPosition.x + TimeToTarget * ComponentVelocity.x,
            TargetPosition.y + TimeToTarget * ComponentVelocity.y,
            TargetPosition.z + TimeToTarget * ComponentVelocity.z + bulletDrop
        };
    }

    auto IsInScreen(fvector2d screen_location) -> bool
    {
        if (screen_location.x > 0 && screen_location.x < globals.ScreenWidth && screen_location.y > 0 && screen_location.y < globals.ScreenHeight) return true;
        else return false;
    }
}; gamehelper_t game_helper;

char* wchar_to_char(const wchar_t* pwchar)
{
    int currentCharIndex = 0;
    char currentChar = pwchar[currentCharIndex];

    while (currentChar != '\0')
    {
        currentCharIndex++;
        currentChar = pwchar[currentCharIndex];
    }

    const int charCount = currentCharIndex + 1;

    char* filePathC = (char*)malloc(sizeof(char) * charCount);

    for (int i = 0; i < charCount; i++)
    {
        char character = pwchar[i];

        *filePathC = character;

        filePathC += sizeof(char);

    }
    filePathC += '\0';

    filePathC -= (sizeof(char) * charCount);

    return filePathC;
}

ImVec2 calc_aim(const fvector2d& target2D, const ImVec2& screenCenter, int aimSpeed, float screenWidth, float screenHeight)
{
    float targetX = 0.0f;
    float targetY = 0.0f;

    if (target2D.x != 0)
    {
        if (target2D.x > screenCenter.x)
        {
            targetX = -(screenCenter.x - target2D.x);
            targetX /= aimSpeed;
            if (targetX + screenCenter.x > screenCenter.x * 2) targetX = 0;
        }

        if (target2D.x < screenCenter.x)
        {
            targetX = target2D.x - screenCenter.x;
            targetX /= aimSpeed;
            if (targetX + screenCenter.x < 0) targetX = 0;
        }
    }
    if (target2D.y != 0)
    {
        if (target2D.y > screenCenter.y)
        {
            targetY = -(screenCenter.y - target2D.y);
            targetY /= aimSpeed;
            if (targetY + screenCenter.y > screenCenter.y * 2) targetY = 0;
        }

        if (target2D.y < screenCenter.y)
        {
            targetY = target2D.y - screenCenter.y;
            targetY /= aimSpeed;
            if (targetY + screenCenter.y < 0) targetY = 0;
        }
    }
    return ImVec2(targetX, targetY);
}

