#include <windows.h> // 使 XMVerifyCPUSupport 函数返回正确值
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
#include "MyMatrix.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator << (ostream& os, FXMVECTOR v)
{
    XMFLOAT4 dest;
    XMStoreFloat4(&dest, v);
    os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
    return os;
}

ostream& XM_CALLCONV operator << (ostream& os, FXMMATRIX m)
{
    for (int i = 0; i < 4; i++)
    {
        os << XMVectorGetX(m.r[i]) << "\t";
        os << XMVectorGetY(m.r[i]) << "\t";
        os << XMVectorGetZ(m.r[i]) << "\t";
        os << XMVectorGetW(m.r[i]) << "\n";
    }
    return os;
}

void DirectXMatrix()
{
    XMMATRIX A (1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 4.0f, 0.0f,
            1.0f, 2.0f, 3.0f, 1.0f);

    XMMATRIX B = XMMatrixIdentity(); // 单位矩阵
    XMMATRIX C = A * B;

    XMMATRIX D = XMMatrixTranspose(A); // 转置矩阵

    XMVECTOR det = XMMatrixDeterminant(A); // 行列式
    XMMATRIX E = XMMatrixInverse(&det, A); // 用行列式和原矩阵求逆矩阵

    XMMATRIX F = A * E;

    cout << "A = " << "\n" << A << "\n";
    cout << "B = " << "\n" << B << "\n";
    cout << "C = A * B" << "\n" << C << "\n";
    cout << "D = A^T" << "\n" << D << "\n";
    cout << "det A = " << det << "\n";
    cout << "E = A^-1" << "\n" << E << "\n";
    cout << "F = A * A^-1" << "\n" << F << "\n";
}

void MyMatrixTest()
{
    auto T = new MyMatrix(3, 3);
    T->Set(9, {
        1.0f, 2.0f, 3.0f,
        2.0f, 1.5f, 2.0f,
        3.0f, 2.0f, 1.0f
    });
    cout << "T = " << "\n" << T << "\n";
    auto TTrans = T->Transpose();
    cout << "TTrans = " << "\n" << TTrans << "\n";
    auto TMinor = T->Minor(0, 0);
    cout << "TMinor = " << "\n" << TMinor << "\n";
    auto TInverse = T->Inverse();
    cout << "TInverse = " << "\n" << TInverse << "\n";
    auto TMult = T->Multiply(TInverse);
    cout << "T * TInverse = " << "\n" << TMult << "\n";
    cout << "det T = " << T->Det() << "\n";
    cout << "det TTrans = " << TTrans->Det() << "\n";
    cout << "det TInverse = " << TInverse->Det() << "\n";
    delete T;
    delete TTrans;
    delete TMinor;
    delete TInverse;
    delete TMult;
}

int main()
{
    if (!XMVerifyCPUSupport())
    {
        cout << "directx math not supported" << "\n";
        return 0;
    }

    DirectXMatrix();

    MyMatrixTest();
    
    return 0;
}
