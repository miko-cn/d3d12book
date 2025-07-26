#include <windows.h> // 为了使XMVerifyCPUSupport函数返回正确值
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

// 重载"<<"运算符，通过cout输出XMVECTOR
ostream& XM_CALLCONV operator<<(ostream& os, FXMVECTOR v)
{
    XMFLOAT3 dest;
    XMStoreFloat3(&dest, v);

    os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
    return os;
}

void XMVectorBasic()
{
    cout << "--- XMVectorBasic ---" << "\n";

    XMVECTOR p = XMVectorZero();
    XMVECTOR q = XMVectorSplatOne();
    XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
    XMVECTOR v = XMVectorReplicate(-2.0f);
    XMVECTOR w = XMVectorSplatZ(u);
    cout << "p = " << p << "\n";
    cout << "q = " << q << "\n";
    cout << "u = " << u << "\n";
    cout << "v = " << v << "\n";
    cout << "w = " << w << "\n";
}

void XMVectorFunc()
{
    cout << "--- XMVectorFunc ---" << "\n";

    XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
    XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
    XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);

    cout << "u = " << u << "\n";
    cout << "v = " << v << "\n";
    cout << "w = " << w << "\n";
    cout << "n = " << n << "\n";

    // 向量加法：利用XMVECTOR类型的加法运算符+
    XMVECTOR a = u + v;
    cout << "a = u + v = " << a << "\n";

    // 向量减法：利用XMVECTOR类型的减法运算符
    XMVECTOR b = u - v;
    cout << "b = u - v = " << b << "\n";

    // 标量乘法：利用XMVECTOR类型的标量乘法运算符*
    XMVECTOR c = 10.0f * u;
    cout << "c = 10.0f * u = " << c << "\n";

    // ||u||
    XMVECTOR L = XMVector3Length(u);
    cout << "L = ||u|| = " << L << "\n";

    // low cost ||u||, low accuracy
    XMVECTOR LEst = XMVector3LengthEst(u);
    cout << "LEst = Est(||u||) = " << LEst << "\n";
        
    // d = u / ||u||
    XMVECTOR d = XMVector3Normalize(u);
    cout << "d = u / ||u|| = " << d << "\n";

    // low cost u/||u||, low accuracy
    XMVECTOR dEst = XMVector3NormalizeEst(u);
    cout << "dEst = Est(u / ||u||) = " << dEst << "\n";

    // s = u dot v
    XMVECTOR s = XMVector3Dot(u, v);
    cout << "s = u dot v = " << s << "\n";

    // e = u x v
    XMVECTOR e = XMVector3Cross(u, v);
    cout << "e = u x v = " << e << "\n";

    // 求出proj_n(w) 和 perp_n(w)
    XMVECTOR projW;
    XMVECTOR perpW;
    XMVector3ComponentsFromNormal(&projW, &perpW, w, n);
    cout << "projW = " << projW << "\n";
    cout << "perpW = " << perpW << "\n";

    // projW + perpW == w?
    bool equal = XMVector3Equal(projW + perpW, w) != 0;
    bool notEqual = XMVector3NotEqual(projW + perpW, w) != 0;
    cout << "projW + perpW == w = " << equal << "\n";
    cout << "projW + perpW != w = " << notEqual << "\n";

    // projW和perpW之间的夹角应为90度
    XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);
    float angleRadians = XMVectorGetX(angleVec);
    float angleDregress = XMConvertToDegrees(angleRadians);
    cout << "angleRadians = " << angleRadians << "\n";
    cout << "angleDregress = " << angleDregress << "\n";
}

void XMVectorFloatError()
{
    cout << "--- XMVectorFloatError ---" << "\n";

    cout.precision(8);  // 设置输出浮点数的有效位数

    XMVECTOR u = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
    XMVECTOR n = XMVector3Normalize(u);
    XMVECTOR ln = XMVector3Length(n);
    float lnv = XMVectorGetX(ln);

    // 在数学上，此向量长度应该是1，在计算机中的数值表达呢？
    cout << "lnv = " << lnv << "\n";
    if (lnv == 1.0f)
    {
        cout << "lnv == 1" << "\n";
    }
    else
    {
        cout << "lnv != 1" << "\n";
    }

    // 1的任意次方都应该是1，但计算机中的情况呢？
    float powlnv = powf(lnv, 1.0e6f);
    cout << "powlnv = lnv ^ (10^6) = " << powlnv << "\n";

    XMVECTOR l1 = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
    XMVECTOR Epsilon = XMVectorSet(0.001f, 0.001f, 0.001f, 0.0f);
    bool res = XMVector3NearEqual(ln, l1, Epsilon);
    cout << "l1 == " << l1 << "\n";
    cout << "ln == " << ln << "\n";
    cout << "Epsilon == " << Epsilon << "\n";
    cout << "XMVector3NearEqual(ln, l1, Epsilon) == " << res << "\n";
}

int main()
{
    cout.setf(ios_base::boolalpha);

    // 检查是否支持SSE2指令集 （Pentium4，AMD K8及其后续版本的处理器）
    if (!XMVerifyCPUSupport())
    {
        cout << "directx math not supported" << "\n";
        return 0;
    }

    XMVectorBasic();

    XMVectorFunc();

    XMVectorFloatError();
    
    return 0;
}
