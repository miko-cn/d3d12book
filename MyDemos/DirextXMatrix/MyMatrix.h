#pragma once
#include <iostream>

class MyMatrix
{
    float* m;
public:
    const int rows, cols;
    MyMatrix(const int in_rows, const int in_cols) : rows(in_rows), cols(in_cols)
    {
        if (rows <= 0 || cols <= 0)
        {
            m = nullptr;
            return;
        }
        const int size = rows * cols;
        m = new float[size];
        memset(m, 0, size * sizeof(float));
    }

    float GetAt(const int row, const int col) const
    {
        return m[row * cols + col];
    }

    void SetAt(const int row, const int col, const float value)
    {
        m[row * cols + col] = value;
    }

    void Set(int count, std::initializer_list<float> nums)
    {
        if (count > 0 && count <= cols * rows)
        {
            int i = 0;
            for (auto &num : nums)
            {
                m[i++] = num;
            }
        }
    }

    MyMatrix* Transpose() const
    {
        if (m == nullptr) return nullptr;
        const auto t = new MyMatrix(cols, rows);
        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                t->SetAt(i, j, GetAt(j, i));
            }
        }
        return t;
    }

    MyMatrix* Minor(const int row, const int col) const
    {
        if (m == nullptr) return nullptr;
        if (row < 0 || col < 0 || row >= cols || col >= rows)
        {
            return nullptr;
        }
        if (cols == 1 || rows == 1)
        {
            // 如果已经是1 x 1，不存在余子阵
            return nullptr;
        }
        const auto t = new MyMatrix(rows - 1, cols - 1);
        for (int i = 0; i < rows; i++)
        {
            if (i == row) continue;
            int mi = i < row ? i : i - 1;
            for (int j = 0; j < cols; j++)
            {
                if (j == col) continue;
                int mj = j < col ? j : j - 1;
                t->SetAt(mi, mj, GetAt(i, j));
            }
        }
        return t;
    }

    float Det()
    {
        if (rows != cols)
        {
            return 0.0f;
        }
        if (rows == 1 && cols == 1)
        {
            return GetAt(0, 0);
        }
        float res = 0.0f;
        for (int i = 0; i < cols; i++)
        {
            auto minor = Minor(0, i);
            auto factor = i % 2 == 0 ? 1.0f : -1.0f;
            res += GetAt(0, i) * factor * minor->Det();
            delete minor;
            minor = nullptr;
        }
        return res;
    }

    MyMatrix* Adjoint()
    {
        if (m == nullptr || rows != cols || rows == 1)
        {
            return nullptr;
        }
        auto t = new MyMatrix(rows, cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                auto minor = Minor(i, j);
                auto factor = (i + j) % 2 == 0 ? 1.0f : -1.0f;
                auto det = minor->Det();
                t->SetAt(i, j, factor * det);
            }
        }
        return t;
    }
    
    MyMatrix* Inverse()
    {
        if (m == nullptr || rows != cols)
        {
            return nullptr;
        }
        if (cols == 1 || rows == 1)
        {
            auto t = new MyMatrix(1, 1);
            t->SetAt(0, 0, GetAt(0, 0));
            auto det = t->Det();
            if (abs(det) < 1e-4)
            {
                delete t;                
                return nullptr;
            }
            t->Division(det);
            return t;
        }
        auto det = Det();
        if (abs(det) < 1e-4)
        {
            return nullptr;
        }
        auto adjoint = Adjoint();
        auto adjointTrans = adjoint->Transpose();
        delete adjoint;
        adjointTrans->Division(det);
        return adjointTrans;
    }

    void print(std::ostream& os)
    {
        if (m == nullptr)
        {
            os << "Invalid Matrix" << "\n";
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                os << GetAt(i, j) << "\t";
            }
            os << "\n";
        }
    }

    ~MyMatrix()
    {
        if (m != nullptr)
        {
            delete m;
            m = nullptr;
        }
    }

    void Multiply(const float fac)
    {
        if (m != nullptr)
        {
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    m[i * rows + j] *= fac;
                }
            }
        }
    }

    void Division(const float fac)
    {
        if (m != nullptr && fac != 0.0f)
        {
            Multiply(1.0f / fac);
        }
    }

    MyMatrix* Multiply(const MyMatrix* rhs) const
    {
        if (rhs == nullptr || rhs->rows != cols)
        {
            return nullptr;
        }
        auto t = new MyMatrix(rows, rhs->cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < rhs->cols; j++)
            {
                float cij = 0.0f;
                for (int k = 0; k < cols; k++)
                {
                    cij += GetAt(i, k) * rhs->GetAt(k, j);
                }
                t->SetAt(i, j, cij);
            }
        }
        return t;
    }
};

inline std::ostream& operator << (std::ostream& os, MyMatrix* m)
{
    if (m != nullptr)
    {
        m->print(os);
    }
    return os;
}