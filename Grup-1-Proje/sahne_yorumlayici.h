#ifndef SAHNE_YORUMLAYICI_H
#define SAHNE_YORUMLAYICI_H

#include "QString"
#include "QFile"
#include "QTextStream"
#include "QStringList"
#include "QMessageBox"

///
/// Sahne Yorumlayıcı ::: Orhan Gazi Hafif
///

int ISIK_SAYI;
int UCGEN_SAYI;
int KURE_SAYI;

float ARKA_R;
float ARKA_G;
float ARKA_B;
int FOV;

int ISIK_NO = 0;
int UCGEN_NO = 0;
int KURE_NO = 0;

float AMBIENT[3];
float DIFFUSE[3];
float REFLECTIVE;
float SPECULAR[4];

float UCGENLER [32] [20]; // ::: [Üçgen Numarası] [koordinatlar(9) - ambient(3) rgb - diffuse(3) rgb - reflective(1) k - speküler(4) rgbp] ::: Sınırlandırma İşletim Sisteminin Bellek adreslemesi için mecburi
float KURELER [32] [20]; // 15'e kadar dolu // ::: [Küre Numarası] [yarıçap uzunluğu{0} ve koordinatlar{1-3 arası}(4)- ambient(3) rgb - diffuse(3) rgb - reflective(1) k - speküler(4) rgbp] ::: Sınırlandırma İşletim Sisteminin Bellek adreslemesi için mecburi
float ISIKLAR [32] [20]; // 6'ya kadar dolu // ::: [Işık numarası] [koordinatlar(3) - renk(3)] ::: Sınırlandırma İşletim Sisteminin Bellek adreslemesi için mecburi

QString streamSagla(QString dosya_adi)
{
    QFile dosya1(dosya_adi);
    if(!dosya1.open(QFile::ReadOnly | QFile::Text)) return "Dosya açılamıyor.";

    QTextStream ts1(&dosya1);
    ts1.setCodec("ISO-8859-9");
    QString str = ts1.readAll();
    dosya1.close();

    if(str.count("  ") > 0 || str.count("   ") > 0 || str.count("    ") > 0) return "X";

    return str;
}

QString sahneYorumla(QString dosya_adi)
{
    QString string_tamami = streamSagla(dosya_adi);
    if (string_tamami == "X") return "Dosya bozuk.";

    ISIK_SAYI = string_tamami.count("light");
    UCGEN_SAYI = string_tamami.count("begin");
    KURE_SAYI = string_tamami.count("sphere");

    if (ISIK_SAYI == 0) return "Işık yok.";

    if (UCGEN_SAYI == 0 && KURE_SAYI == 0) return "Sahnede tanımlı nesne yok.";

    QStringList satirlar = string_tamami.split('\n');
    int satir_sayisi = satirlar.count();
    QStringList kelimeler;
    //int kelime_sayisi;
    for(int i = 0; i < satir_sayisi; i++)
    {
        kelimeler = satirlar[i].split(" ");
        //kelime_sayisi = kelimeler.count();

        if(kelimeler[0] == "") continue;
        if(kelimeler[0] == "#") continue;
        if(kelimeler[0] == "fov")
        {
            FOV = kelimeler[1].toFloat();
        }
        if(kelimeler[0] == "background")
        {
            ARKA_R = kelimeler[1].toFloat();
            ARKA_G = kelimeler[2].toFloat();
            ARKA_B = kelimeler[3].toFloat();
        }
        if(kelimeler[0] == "light")
        {
            ISIKLAR[ISIK_NO][0] = kelimeler[1].toFloat();
            ISIKLAR[ISIK_NO][1] = kelimeler[2].toFloat();
            ISIKLAR[ISIK_NO][2] = kelimeler[3].toFloat();
            ISIKLAR[ISIK_NO][3] = kelimeler[4].toFloat();
            ISIKLAR[ISIK_NO][4] = kelimeler[5].toFloat();
            ISIKLAR[ISIK_NO][5] = kelimeler[6].toFloat();

            ISIK_NO++;
        }
        if(kelimeler[0] == "ambient")
        {
            AMBIENT[0] = kelimeler[1].toFloat();
            AMBIENT[1] = kelimeler[2].toFloat();
            AMBIENT[2] = kelimeler[3].toFloat();
        }
        if(kelimeler[0] == "diffuse")
        {
            DIFFUSE[0] = kelimeler[1].toFloat();
            DIFFUSE[1] = kelimeler[2].toFloat();
            DIFFUSE[2] = kelimeler[3].toFloat();
        }
        if(kelimeler[0] == "specular")
        {
            SPECULAR[0] = kelimeler[1].toFloat();
            SPECULAR[1] = kelimeler[2].toFloat();
            SPECULAR[2] = kelimeler[3].toFloat();
            SPECULAR[3] = kelimeler[4].toFloat();
        }
        if(kelimeler[0] == "reflective") REFLECTIVE = kelimeler[1].toFloat();
        if(kelimeler[0] == "sphere")
        {
            KURELER[KURE_NO][0] = kelimeler[1].toFloat();
            KURELER[KURE_NO][1] = kelimeler[2].toFloat();
            KURELER[KURE_NO][2] = kelimeler[3].toFloat();
            KURELER[KURE_NO][4] = kelimeler[4].toFloat();

            KURELER[KURE_NO][5] = AMBIENT[0];
            KURELER[KURE_NO][6] = AMBIENT[1];
            KURELER[KURE_NO][7] = AMBIENT[2];

            KURELER[KURE_NO][8] = DIFFUSE[0];
            KURELER[KURE_NO][9] = DIFFUSE[1];
            KURELER[KURE_NO][10] = DIFFUSE[2];

            KURELER[KURE_NO][11] = REFLECTIVE;

            KURELER[KURE_NO][12] = SPECULAR[0];
            KURELER[KURE_NO][13] = SPECULAR[1];
            KURELER[KURE_NO][14] = SPECULAR[2];
            KURELER[KURE_NO][15] = SPECULAR[3];

            KURE_NO++;
        }
        if(kelimeler[0] == "begin") // Sadece üçgen olduğu için bu şekilde tasarladım. Poligon köşesi saydırmakla uğraşmadım.
        {
            kelimeler = satirlar[++i].split(" ");

            UCGENLER[UCGEN_NO][0] = kelimeler[1].toFloat();
            UCGENLER[UCGEN_NO][1] = kelimeler[2].toFloat();
            UCGENLER[UCGEN_NO][2] = kelimeler[3].toFloat();

            kelimeler = satirlar[++i].split(" ");

            UCGENLER[UCGEN_NO][3] = kelimeler[1].toFloat();
            UCGENLER[UCGEN_NO][4] = kelimeler[2].toFloat();
            UCGENLER[UCGEN_NO][5] = kelimeler[3].toFloat();

            kelimeler = satirlar[++i].split(" ");

            UCGENLER[UCGEN_NO][6] = kelimeler[1].toFloat();
            UCGENLER[UCGEN_NO][7] = kelimeler[2].toFloat();
            UCGENLER[UCGEN_NO][8] = kelimeler[3].toFloat();

            UCGENLER[UCGEN_NO][9] = AMBIENT[0];
            UCGENLER[UCGEN_NO][10] = AMBIENT[1];
            UCGENLER[UCGEN_NO][11] = AMBIENT[2];

            UCGENLER[UCGEN_NO][12] = DIFFUSE[0];
            UCGENLER[UCGEN_NO][13] = DIFFUSE[1];
            UCGENLER[UCGEN_NO][14] = DIFFUSE[2];

            UCGENLER[UCGEN_NO][15] = REFLECTIVE;

            UCGENLER[UCGEN_NO][16] = SPECULAR[0];
            UCGENLER[UCGEN_NO][17] = SPECULAR[1];
            UCGENLER[UCGEN_NO][18] = SPECULAR[2];
            UCGENLER[UCGEN_NO][19] = SPECULAR[3];

            i++;
            UCGEN_NO++;
        }
        if(kelimeler[0] == "write") return "Sahme okuma başarılı.";
    }
    return "Okumada bir hata oluştu.";
}

#endif // SAHNE_YORUMLAYICI_H
