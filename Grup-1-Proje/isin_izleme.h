#ifndef ISIN_IZLEME_H
#define ISIN_IZLEME_H

#include <QtCore/qmath.h>
#include "QPixmap"
#include "QMatrix"
#include "QImage"
#include "QColor"
#include "QPainter"
#include "sahne_yorumlayici.h"

///
/// Lineer Cebir, 3b Dönüşümler ::: Ahmet Saydam
///
/// Işın İzleme ::: Orhan Gazi Hafif ::: Lokman Dursun 
///

int EKR_X = 300;
int EKR_Y = 300;

float EKRAN [300] [300] [6]; // [x][y]  [R, G, B, Nx, Ny, Nz]

int GD_X = 10;
int GD_Y = 10;

float t_en_kucuk = .0; // Arka yüzeyleri kaldırmak için.
int T_UCGEN_NO = 0;

float ORJ_UZAKLIK_Z = -8.66;

float SIFIRA_YAKLASIM = 0.00001;

float S, S1, S2, S3, t; //Işın üçgen kesişimi için her seferde hafıza ayırma olmasın diye.

float iREFLEKTIF;

void ekranTemizle()
{
    for (int satir = 0; satir < 32; satir++)
    {
        for(int sutun = 0; sutun < 20; sutun++)
        {
            UCGENLER [satir] [sutun] = NULL;
            KURELER [satir] [sutun] = NULL;
            ISIKLAR [satir] [sutun] = NULL;
        }
    }
    ISIK_SAYI = 0;
    UCGEN_SAYI = 0;
    KURE_SAYI = 0;

    ISIK_NO = 0;
    UCGEN_NO = 0;
    KURE_NO = 0;
}

void sahneTemizle()
{
    for (int satir = 0; satir < EKR_Y; satir++)
    {
        for(int sutun = 0; sutun < EKR_X; sutun++)
        {
            for(int i = 0; i < 6; i++)
            {
                EKRAN[satir][sutun] [i] = NULL;
            }
        }
    }
}

class Vektor
{
public:
    float x, y, z;
    Vektor(){}
    Vektor(float xd, float yd, float zd)
    {
        x = xd;
        y = yd;
        z = zd;
    }
}RD, R, R_KESISIM, I, iDIF, iSPEC, iAMB, r_vek, v_vek; //Işın üçgen kesişiminde her sefer memory allocation olmaması için.

Vektor R0(0, 0, 0);

class Nokta2b
{
public:
    float x, y;
    Nokta2b(){}
    Nokta2b(float xd, float yd)
    {
        x = xd;
        y = yd;
    }

}nokta;

Vektor skalerIleVektorCarpim(Vektor v, float sabit)
{
    v.x = v.x*sabit;
    v.y = v.y*sabit;
    v.z = v.z*sabit;
    return v;
}

Vektor skalerIleVektorBol(Vektor v, float sabit)
{
    v.x = v.x/sabit;
    v.y = v.y/sabit;
    v.z = v.z/sabit;
    return v;
}

Vektor normalizeEt(Vektor vektor)
{
    float x = vektor.x;
    float y = vektor.y;
    float z = vektor.z;
    float vektor_uzunluk = qSqrt( (x*x)+(y*y)+(z*z) );
    vektor.x = x/vektor_uzunluk;
    vektor.y = y/vektor_uzunluk;
    vektor.z = z/vektor_uzunluk;
    return vektor;
}

Vektor vektorelCarpim(Vektor A, Vektor B)
{
    return Vektor( (A.y*B.z) - (A.z*B.y), (A.z*B.x) - (A.x*B.z), (A.x*B.y) - (A.y*B.x) );
}

float vektorSkalerCarpim(Vektor A, Vektor B)
{
    return (A.x*B.x) + (A.y*B.y) + (A.z*B.z);
}

float vektorBoyBul(Vektor vector)
{
    float x = vector.x;
    float y = vector.y;
    float z = vector.z;
    float vektor_uzunluk = qSqrt( (x*x)+(y*y)+(z*z) );
    return vektor_uzunluk;
}

Nokta2b ekrandanGoruntuDuzlemine(Nokta2b gelen_nokta)
{
    return Nokta2b(( GD_X*gelen_nokta.x/(EKR_X) ) - (GD_X/2), ( (GD_Y/2) - (GD_Y*gelen_nokta.y/(EKR_Y)) ));
}

Vektor vektorElemanlariCarp(Vektor A, Vektor B)
{
    return Vektor(A.x * B.x, A.y * B.y, A.z * B.z);
}

Vektor vektorCikarIkidenBiri(Vektor A, Vektor B)
{
    return Vektor(B.x - A.x, B.y - A.y, B.z - A.z);
}

Vektor ucgenNormalBul(Vektor A, Vektor B, Vektor C)
{
    return vektorelCarpim( vektorCikarIkidenBiri(C, A), vektorCikarIkidenBiri(C, B) );
}

Vektor vektorTopla(Vektor A, Vektor B)
{
    return Vektor(A.x + B.x, A.y + B.y, A.z + B.z);
}

class Ucgen
{
public:
    float t, reflektif, spec_Us;
    Vektor normal, v0, v1, v2, m_dif, ambient_vek, spec_vek, iSPEC, iDIF;
    int ucgen_numarasi;
    Ucgen(){}
    Ucgen(int no)
    {
        v0.x = UCGENLER[no][0];
        v0.y = UCGENLER[no][1];
        v0.z = UCGENLER[no][2];
        v1.x = UCGENLER[no][3];
        v1.y = UCGENLER[no][4];
        v1.z = UCGENLER[no][5];
        v2.x = UCGENLER[no][6];
        v2.y = UCGENLER[no][7];
        v2.z = UCGENLER[no][8];

        ambient_vek.x = UCGENLER[no][9];
        ambient_vek.y = UCGENLER[no][10];
        ambient_vek.z = UCGENLER[no][11];

        m_dif.x = UCGENLER[no][12];
        m_dif.y = UCGENLER[no][13];
        m_dif.z = UCGENLER[no][14];

        reflektif = UCGENLER[no][15];

        spec_vek.x  = UCGENLER[no][16];
        spec_vek.y  = UCGENLER[no][17];
        spec_vek.z  = UCGENLER[no][18];

        spec_Us = UCGENLER[no][19];

        ucgen_numarasi = no;
        normal = ucgenNormalBul(v0, v1, v2);
    }
}UCGEN,uc;

float fark;

class Isik
{
public:
    float x, y, z, r, g, b;
    Isik(){}
    Isik(float X, float Y, float Z, float R, float G, float B)
    {
        r = R;
        g = G;
        b = B;
        x = -X;
        y = -Y;
        z = -Z;
    }
};

float isinUcgenKesisimVarMi(Vektor r0, Vektor rD, Ucgen u, Isik isik)
{
    float D = -(    vektorSkalerCarpim(u.v0, u.normal)    );
    float T = -(    (vektorSkalerCarpim(r0, u.normal) + D)  /  vektorSkalerCarpim(rD, u.normal)   );

    if(T > 0)
    {
        R_KESISIM = Vektor(R0.x + (T*RD.x), R0.y + (T*RD.y), R0.z + (T*RD.z));

        I = normalizeEt(   vektorCikarIkidenBiri( R_KESISIM, Vektor(isik.x, isik.y, isik.z) )   );

        iDIF = skalerIleVektorCarpim(  vektorElemanlariCarp(u.m_dif, Vektor(isik.r, isik.g, isik.b)), vektorSkalerCarpim( I, normalizeEt(u.normal) )  );

        r_vek = vektorCikarIkidenBiri(skalerIleVektorCarpim(u.normal, 2*(vektorSkalerCarpim(I, u.normal))), I);

        v_vek = normalizeEt(vektorCikarIkidenBiri(R_KESISIM, R0));

        iSPEC = skalerIleVektorCarpim(vektorElemanlariCarp(u.m_dif, Vektor(isik.r, isik.g, isik.b)), qPow(vektorSkalerCarpim(r_vek, v_vek), u.spec_Us));

       /* QString str = QString::number(I.x, 'g', 6) + ", " + QString::number(I.y, 'g', 6) + ", " + QString::number(I.z, 'g', 6);
        QMessageBox msb;
        msb.setText(str);
        msb.exec();*/

        S1 = 0.5 * vektorBoyBul( vektorelCarpim(vektorCikarIkidenBiri(u.v0, R_KESISIM), vektorCikarIkidenBiri(u.v0, u.v2)) );
        S2 = 0.5 * vektorBoyBul( vektorelCarpim(vektorCikarIkidenBiri(u.v0, u.v1), vektorCikarIkidenBiri(u.v0, R_KESISIM)) );
        S3 = 0.5 * vektorBoyBul( vektorelCarpim(vektorCikarIkidenBiri(R_KESISIM, u.v1), vektorCikarIkidenBiri(R_KESISIM, u.v2)) );
        S = 0.5 * vektorBoyBul( vektorelCarpim( vektorCikarIkidenBiri(u.v0, u.v1), vektorCikarIkidenBiri(u.v0, u.v2) ) );

        fark = (float)(S - (S1+S2+S3));
        if ((fark <= SIFIRA_YAKLASIM && fark >= 0.0) || (fark >= -SIFIRA_YAKLASIM && fark <= 0.0)) return T; else return .0;
    }
    else return .0;
}

class Kure
{
public:
    Vektor merkez;
    float yaricap;
    Kure(){}
    Kure(Vektor m, float r)
    {
        merkez = m;
        yaricap = r;
    }
};

void isinIzleUcgenleri(Isik isik)
{
    for (int satir = 0; satir < EKR_Y; satir++)
    {
        for(int sutun = 0; sutun < EKR_X; sutun++)
        {
            nokta = Nokta2b(satir, sutun);
            nokta = ekrandanGoruntuDuzlemine(nokta);
            R = Vektor(nokta.x, nokta.y, ORJ_UZAKLIK_Z);
            RD = normalizeEt(Vektor(skalerIleVektorBol(vektorCikarIkidenBiri(R0, R), vektorBoyBul(R))));

            t_en_kucuk = .0;
            t = .0;

            for(int i = 0; i < UCGEN_SAYI; i++)
            {
                uc = Ucgen(i);
                t = isinUcgenKesisimVarMi(R0, RD, uc, isik);
                if (t_en_kucuk == .0 && t > 0)
                {
                     t_en_kucuk = t;
                     T_UCGEN_NO = uc.ucgen_numarasi;
                     UCGEN = uc;
                     UCGEN.iDIF = iDIF;
                     UCGEN.iSPEC = iSPEC;
                }

                if( t < t_en_kucuk && t_en_kucuk > 0 && t != 0.0)
                {
                    t_en_kucuk = t;
                    T_UCGEN_NO = uc.ucgen_numarasi;
                    UCGEN = uc;
                    UCGEN.iDIF = iDIF;
                    UCGEN.iSPEC = iSPEC;
                }
                if (t == t_en_kucuk && t_en_kucuk != 0.0)
                {
                    EKRAN[satir][sutun][0] = ((UCGEN.iDIF.x + UCGEN.iSPEC.x)*0.4) + (isik.r*0.6);
                    EKRAN[satir][sutun][1] = ((UCGEN.iDIF.y + UCGEN.iSPEC.y)*0.4) + (isik.g*0.6);
                    EKRAN[satir][sutun][2] = ((UCGEN.iDIF.z + UCGEN.iSPEC.z)*0.4) + (isik.b*0.6);
                    EKRAN[satir][sutun][3] = UCGEN.normal.x;
                    EKRAN[satir][sutun][4] = UCGEN.normal.y;
                    EKRAN[satir][sutun][5] = UCGEN.normal.z;
                }

            }
        }
    }
}

void resimOlustur(QString sahne_adi)
{
    QPixmap piksel_haritasi( 300, 300 );
    QPainter ressam( &piksel_haritasi );
    QColor backkk;

    backkk.setRedF(ARKA_R);
    backkk.setGreenF(ARKA_G);
    backkk.setBlueF(ARKA_B);
    piksel_haritasi.fill(backkk);

    QColor renk;

    for(int satir = 0; satir < EKR_X; satir++)
    {
        for(int sutun = 0; sutun < EKR_Y; sutun++)
        {
            renk.setRedF(EKRAN[satir][sutun][0]);
            renk.setGreenF(EKRAN[satir][sutun][1]);
            renk.setBlueF(EKRAN[satir][sutun][2]);
            ressam.setPen( renk );
            ressam.drawPoint(satir, sutun);

            if(EKRAN[satir][sutun][0] == NULL)
            {
                renk.setRedF(ARKA_R);
                renk.setGreenF(ARKA_G);
                renk.setBlueF(ARKA_B);
                ressam.setPen( renk );
                ressam.drawPoint(satir, sutun);
            }
        }
    }
    piksel_haritasi.save( ""+ sahne_adi +".png" );
}

#endif // RAY_TRACING_H















