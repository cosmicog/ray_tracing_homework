#ifndef ISIN_IZLEME_H
#define ISIN_IZLEME_H

#include <QtCore/qmath.h>
#include "QPixmap"
#include "QMatrix"
#include "QImage"
#include "QColor"
#include "QPainter"
#include "stage_interpreter.h"

///
/// Ray Tracing ::: Orhan Gazi Hafif ::: Lokman Dursun
///

int SCREEN_X = 300;
int SCREEN_Y = 300;

float SCREEN [300] [300] [6]; // [x][y]  [R, G, B, Nx, Ny, Nz]

int IMG_PLANE_X = 10;
int IMG_PLANE_Y = 10;

float SMALLEST_T = .0; // Arka yüzeyleri kaldırmak için.
int T_TRIANGLE_IND = 0;

float DIST_TO_ORIGIN = -8.66;

float ZERO_EQUIVALENT = 0.00001;

float S, S1, S2, S3, t; //Işın üçgen kesişimi için her seferde hafıza ayırma olmasın diye.

float i_REFLECTIVE;

void clearScreen()
{
    for (int row = 0; row < 32; row++)
    {
        for (int column = 0; column < 20; column++)
        {
            TRIANGLES [row] [column] = .0;
            SPHERES [row] [column] = .0;
            LIGHTS [row] [column] = .0;
        }
    }
    LIGHT_COUNT = 0;
    TRIANGLE_COUNT = 0;
    SPHERE_COUNT = 0;

    LIGHT_IND = 0;
    TRIANGLE_IND = 0;
    SPHERE_IND = 0;
}

void clearStage()
{
    for (int row = 0; row < SCREEN_Y; row++)
    {
        for (int column = 0; column < SCREEN_X; column++)
        {
            for (int i = 0; i < 6; i++)
            {
                SCREEN[row][column] [i] = .0;
            }
        }
    }
}

class Vector3d
{
public:
    float x, y, z;
    Vector3d() {}
    Vector3d(float xd, float yd, float zd)
    {
        x = xd;
        y = yd;
        z = zd;
    }
} RD, R, R_INTERSECTION, I, iDIF, iSPEC, iAMB, r_vec, v_vec; //Işın üçgen kesişiminde her sefer memory allocation olmaması için.

Vector3d R0(0, 0, 0);

class Point2d
{
public:
    float x, y;
    Point2d() {}
    Point2d(float xd, float yd)
    {
        x = xd;
        y = yd;
    }

} POINT;

Vector3d multiplyVectorByScalar(Vector3d v, float scalar)
{
    v.x = v.x * scalar;
    v.y = v.y * scalar;
    v.z = v.z * scalar;
    return v;
}

Vector3d divideVectorToScalar(Vector3d v, float scalar)
{
    v.x = v.x / scalar;
    v.y = v.y / scalar;
    v.z = v.z / scalar;
    return v;
}

float lengthOfVector(Vector3d v)
{
    return qSqrt( (v.x * v.x) + (v.y * v.y) + (v.z * v.z) );;
}

Vector3d normalize(Vector3d v)
{
    float len = lengthOfVector(v);
    v.x = v.x / len;
    v.y = v.y / len;
    v.z = v.z / len;
    return v;
}

Vector3d crossProduct(Vector3d A, Vector3d B)
{
    return Vector3d( (A.y * B.z) - (A.z * B.y), (A.z * B.x) - (A.x * B.z), (A.x * B.y) - (A.y * B.x) );
}

float scalarProduct(Vector3d A, Vector3d B)
{
    return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
}

Point2d screenToImagePlane(Point2d p)
{
    return Point2d(( IMG_PLANE_X * p.x / (SCREEN_X) ) - (IMG_PLANE_X / 2), ( (IMG_PLANE_Y / 2) - (IMG_PLANE_Y * p.y / (SCREEN_Y)) ));
}

Vector3d multipliedVector(Vector3d A, Vector3d B)
{
    return Vector3d(A.x * B.x, A.y * B.y, A.z * B.z);
}

Vector3d subtractFirstFromSecond(Vector3d A, Vector3d B)
{
    return Vector3d(B.x - A.x, B.y - A.y, B.z - A.z);
}

Vector3d planeNormal(Vector3d A, Vector3d B, Vector3d C)
{
    return crossProduct( subtractFirstFromSecond(C, A), subtractFirstFromSecond(C, B) );
}

Vector3d addition(Vector3d A, Vector3d B)
{
    return Vector3d(A.x + B.x, A.y + B.y, A.z + B.z);
}

class Triangle
{
public:
    float t, reflective, spec_Us;
    Vector3d normal, v0, v1, v2, m_dif, ambient_vec, spec_vec, iSPEC, iDIF;
    int triangle_id;
    Triangle() {}
    Triangle(int i)
    {
        v0.x = TRIANGLES[i][0];
        v0.y = TRIANGLES[i][1];
        v0.z = TRIANGLES[i][2];
        v1.x = TRIANGLES[i][3];
        v1.y = TRIANGLES[i][4];
        v1.z = TRIANGLES[i][5];
        v2.x = TRIANGLES[i][6];
        v2.y = TRIANGLES[i][7];
        v2.z = TRIANGLES[i][8];

        ambient_vec.x = TRIANGLES[i][9];
        ambient_vec.y = TRIANGLES[i][10];
        ambient_vec.z = TRIANGLES[i][11];

        m_dif.x = TRIANGLES[i][12];
        m_dif.y = TRIANGLES[i][13];
        m_dif.z = TRIANGLES[i][14];

        reflective = TRIANGLES[i][15];

        spec_vec.x  = TRIANGLES[i][16];
        spec_vec.y  = TRIANGLES[i][17];
        spec_vec.z  = TRIANGLES[i][18];

        spec_Us = TRIANGLES[i][19];

        triangle_id = i;
        normal = planeNormal(v0, v1, v2);
    }
} TRIANGLE, tria;

float difference;

class Light
{
public:
    float x, y, z, r, g, b;
    Light() {}
    Light(float X, float Y, float Z, float R, float G, float B)
    {
        r = R;
        g = G;
        b = B;
        x = -X;
        y = -Y;
        z = -Z;
    }
};

float isRayCrossingWithPlane(Vector3d r0, Vector3d rD, Triangle tr, Light light)
{
    float D = -( scalarProduct(tr.v0, tr.normal) );
    float T = -( (scalarProduct(r0, tr.normal) + D) / scalarProduct(rD, tr.normal) );

    if (T > 0)
    {
        R_INTERSECTION = Vector3d(R0.x + (T * RD.x), R0.y + (T * RD.y), R0.z + (T * RD.z));

        I = normalize(   subtractFirstFromSecond( R_INTERSECTION, Vector3d(light.x, light.y, light.z) )   );

        iDIF = multiplyVectorByScalar(  multipliedVector(tr.m_dif, Vector3d(light.r, light.g, light.b)), scalarProduct( I, normalize(tr.normal) )  );

        r_vec = subtractFirstFromSecond(multiplyVectorByScalar(tr.normal, 2 * (scalarProduct(I, tr.normal))), I);

        v_vec = normalize(subtractFirstFromSecond(R_INTERSECTION, R0));

        iSPEC = multiplyVectorByScalar(multipliedVector(tr.m_dif, Vector3d(light.r, light.g, light.b)), qPow(scalarProduct(r_vec, v_vec), tr.spec_Us));

        /* QString str = QString::number(I.x, 'g', 6) + ", " + QString::number(I.y, 'g', 6) + ", " + QString::number(I.z, 'g', 6);
         QMessageBox msb;
         msb.setText(str);
         msb.exec();*/

        S1 = 0.5 * lengthOfVector( crossProduct(subtractFirstFromSecond(tr.v0, R_INTERSECTION), subtractFirstFromSecond(tr.v0, tr.v2)) );
        S2 = 0.5 * lengthOfVector( crossProduct(subtractFirstFromSecond(tr.v0, tr.v1), subtractFirstFromSecond(tr.v0, R_INTERSECTION)) );
        S3 = 0.5 * lengthOfVector( crossProduct(subtractFirstFromSecond(R_INTERSECTION, tr.v1), subtractFirstFromSecond(R_INTERSECTION, tr.v2)) );
        S = 0.5 * lengthOfVector( crossProduct( subtractFirstFromSecond(tr.v0, tr.v1), subtractFirstFromSecond(tr.v0, tr.v2) ) );

        difference = (float)(S - (S1 + S2 + S3));
        if ((difference <= ZERO_EQUIVALENT && difference >= 0.0) || (difference >= -ZERO_EQUIVALENT && difference <= 0.0)) return T;
        else return .0;
    }
    else return .0;
}

/**
 * @brief The Sphere class, dummy
 */
class Sphere
{
public:
    Vector3d center;
    float radius;
    Sphere() {}
    Sphere(Vector3d m, float r)
    {
        center = m;
        radius = r;
    }
};

void rayTraceTriangles(Light light)
{
    for (int row = 0; row < SCREEN_Y; row++)
    {
        for (int column = 0; column < SCREEN_X; column++)
        {
            POINT = Point2d(row, column);
            POINT = screenToImagePlane(POINT);
            R = Vector3d(POINT.x, POINT.y, DIST_TO_ORIGIN);
            RD = normalize(Vector3d(divideVectorToScalar(subtractFirstFromSecond(R0, R), lengthOfVector(R))));

            SMALLEST_T = .0;
            t = .0;

            for (int i = 0; i < TRIANGLE_COUNT; i++)
            {
                tria = Triangle(i);
                t = isRayCrossingWithPlane(R0, RD, tria, light);
                if (SMALLEST_T == .0 && t > 0)
                {
                    SMALLEST_T = t;
                    T_TRIANGLE_IND = tria.triangle_id;
                    TRIANGLE = tria;
                    TRIANGLE.iDIF = iDIF;
                    TRIANGLE.iSPEC = iSPEC;
                }
                else if ( t < SMALLEST_T && SMALLEST_T > 0 && t != 0.0)
                {
                    SMALLEST_T = t;
                    T_TRIANGLE_IND = tria.triangle_id;
                    TRIANGLE = tria;
                    TRIANGLE.iDIF = iDIF;
                    TRIANGLE.iSPEC = iSPEC;
                }
                if (t == SMALLEST_T && SMALLEST_T != 0.0)
                {
                    SCREEN[row][column][0] = ((TRIANGLE.iDIF.x + TRIANGLE.iSPEC.x) * 0.4) + (light.r * 0.6);
                    SCREEN[row][column][1] = ((TRIANGLE.iDIF.y + TRIANGLE.iSPEC.y) * 0.4) + (light.g * 0.6);
                    SCREEN[row][column][2] = ((TRIANGLE.iDIF.z + TRIANGLE.iSPEC.z) * 0.4) + (light.b * 0.6);
                    SCREEN[row][column][3] = TRIANGLE.normal.x;
                    SCREEN[row][column][4] = TRIANGLE.normal.y;
                    SCREEN[row][column][5] = TRIANGLE.normal.z;
                }

            }
        }
    }
}

void createPicture(QString stage_name)
{
    QPixmap pm( 300, 300 );
    QPainter bob_ross( &pm );
    QColor backkk;

    backkk.setRedF(BACK_R);
    backkk.setGreenF(BACK_G);
    backkk.setBlueF(BACK_B);
    pm.fill(backkk);

    QColor c;

    for (int row = 0; row < SCREEN_X; row++)
    {
        for (int column = 0; column < SCREEN_Y; column++)
        {
            c.setRedF(SCREEN[row][column][0]);
            c.setGreenF(SCREEN[row][column][1]);
            c.setBlueF(SCREEN[row][column][2]);
            bob_ross.setPen( c );
            bob_ross.drawPoint(row, column);

            if (SCREEN[row][column][0] == .0)
            {
                c.setRedF(BACK_R);
                c.setGreenF(BACK_G);
                c.setBlueF(BACK_B);
                bob_ross.setPen( c );
                bob_ross.drawPoint(row, column);
            }
        }
    }
    pm.save( "" + stage_name + ".png" );
}

#endif // RAY_TRACING_H















