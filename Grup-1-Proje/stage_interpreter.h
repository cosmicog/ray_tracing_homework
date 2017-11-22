#ifndef STAGE_INTERPRETER_H
#define STAGE_INTERPRETER_H

#include "QString"
#include "QFile"
#include "QTextStream"
#include "QStringList"
#include "QMessageBox"

///
/// Scene Interpreter ::: Orhan Gazi Hafif
///

int LIGHT_COUNT;
int TRIANGLE_COUNT;
int SPHERE_COUNT;

float BACK_R;
float BACK_G;
float BACK_B;
int FOV;

int LIGHT_IND = 0;
int TRIANGLE_IND = 0;
int SPHERE_IND = 0;

float AMBIENT[3];
float DIFFUSE[3];
float REFLECTIVE;
float SPECULAR[4];

float TRIANGLES [256] [20]; // ::: [Üçgen Numarası] [koordinatlar(9) - ambient(3) rgb - diffuse(3) rgb - reflective(1) k - speküler(4) rgbp] ::: Sınırlandırma İşletim Sisteminin Bellek adreslemesi için mecburi
float SPHERES [256] [20]; // 15'e kadar dolu // ::: [Küre Numarası] [yarıçap uzunluğu{0} ve koordinatlar{1-3 arası}(4)- ambient(3) rgb - diffuse(3) rgb - reflective(1) k - speküler(4) rgbp] ::: Sınırlandırma İşletim Sisteminin Bellek adreslemesi için mecburi
float LIGHTS [256] [20]; // 6'ya kadar dolu // ::: [Işık numarası] [koordinatlar(3) - renk(3)] ::: Sınırlandırma İşletim Sisteminin Bellek adreslemesi için mecburi

QString supplyStream(QString file_name)
{
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return "Unable to open file!";

    QTextStream ts(&file);
    ts.setCodec("ISO-8859-9");
    QString str = ts.readAll();
    file.close();

    if (str.count("  ") > 0 || str.count("   ") > 0 || str.count("    ") > 0) return "X";

    return str;
}

QString interpretStage(QString file_name)
{
    QString string_all = supplyStream(file_name);
    if (string_all == "X") return "File is corrupted.";

    LIGHT_COUNT = string_all.count("light");
    TRIANGLE_COUNT = string_all.count("begin");
    SPHERE_COUNT = string_all.count("sphere");

    if (LIGHT_COUNT == 0) return "No light...";

    if (TRIANGLE_COUNT == 0 && SPHERE_COUNT == 0) return "There are no known objects in the stage.";

    QStringList rows = string_all.split('\n');
    int row_count = rows.count();
    QStringList words;
    //int kelime_sayisi;
    for (int i = 0; i < row_count; i++)
    {
        words = rows[i].split(" ");
        //kelime_sayisi = kelimeler.count();

        if (words[0] == "") continue;
        if (words[0] == "#") continue;
        if (words[0] == "fov")
        {
            FOV = words[1].toFloat();
        }
        if (words[0] == "background")
        {
            BACK_R = words[1].toFloat();
            BACK_G = words[2].toFloat();
            BACK_B = words[3].toFloat();
        }
        if (words[0] == "light")
        {
            LIGHTS[LIGHT_IND][0] = words[1].toFloat();
            LIGHTS[LIGHT_IND][1] = words[2].toFloat();
            LIGHTS[LIGHT_IND][2] = words[3].toFloat();
            LIGHTS[LIGHT_IND][3] = words[4].toFloat();
            LIGHTS[LIGHT_IND][4] = words[5].toFloat();
            LIGHTS[LIGHT_IND][5] = words[6].toFloat();

            LIGHT_IND++;
        }
        if (words[0] == "ambient")
        {
            AMBIENT[0] = words[1].toFloat();
            AMBIENT[1] = words[2].toFloat();
            AMBIENT[2] = words[3].toFloat();
        }
        if (words[0] == "diffuse")
        {
            DIFFUSE[0] = words[1].toFloat();
            DIFFUSE[1] = words[2].toFloat();
            DIFFUSE[2] = words[3].toFloat();
        }
        if (words[0] == "specular")
        {
            SPECULAR[0] = words[1].toFloat();
            SPECULAR[1] = words[2].toFloat();
            SPECULAR[2] = words[3].toFloat();
            SPECULAR[3] = words[4].toFloat();
        }
        if (words[0] == "reflective") REFLECTIVE = words[1].toFloat();
        if (words[0] == "sphere")
        {
            SPHERES[SPHERE_IND][0] = words[1].toFloat();
            SPHERES[SPHERE_IND][1] = words[2].toFloat();
            SPHERES[SPHERE_IND][2] = words[3].toFloat();
            SPHERES[SPHERE_IND][4] = words[4].toFloat();

            SPHERES[SPHERE_IND][5] = AMBIENT[0];
            SPHERES[SPHERE_IND][6] = AMBIENT[1];
            SPHERES[SPHERE_IND][7] = AMBIENT[2];

            SPHERES[SPHERE_IND][8] = DIFFUSE[0];
            SPHERES[SPHERE_IND][9] = DIFFUSE[1];
            SPHERES[SPHERE_IND][10] = DIFFUSE[2];

            SPHERES[SPHERE_IND][11] = REFLECTIVE;

            SPHERES[SPHERE_IND][12] = SPECULAR[0];
            SPHERES[SPHERE_IND][13] = SPECULAR[1];
            SPHERES[SPHERE_IND][14] = SPECULAR[2];
            SPHERES[SPHERE_IND][15] = SPECULAR[3];

            SPHERE_IND++;
        }
        if (words[0] == "begin") // Sadece üçgen olduğu için bu şekilde tasarladım. Poligon köşesi saydırmakla uğraşmadım.
        {
            words = rows[++i].split(" ");

            TRIANGLES[TRIANGLE_IND][0] = words[1].toFloat();
            TRIANGLES[TRIANGLE_IND][1] = words[2].toFloat();
            TRIANGLES[TRIANGLE_IND][2] = words[3].toFloat();

            words = rows[++i].split(" ");

            TRIANGLES[TRIANGLE_IND][3] = words[1].toFloat();
            TRIANGLES[TRIANGLE_IND][4] = words[2].toFloat();
            TRIANGLES[TRIANGLE_IND][5] = words[3].toFloat();

            words = rows[++i].split(" ");

            TRIANGLES[TRIANGLE_IND][6] = words[1].toFloat();
            TRIANGLES[TRIANGLE_IND][7] = words[2].toFloat();
            TRIANGLES[TRIANGLE_IND][8] = words[3].toFloat();

            TRIANGLES[TRIANGLE_IND][9] = AMBIENT[0];
            TRIANGLES[TRIANGLE_IND][10] = AMBIENT[1];
            TRIANGLES[TRIANGLE_IND][11] = AMBIENT[2];

            TRIANGLES[TRIANGLE_IND][12] = DIFFUSE[0];
            TRIANGLES[TRIANGLE_IND][13] = DIFFUSE[1];
            TRIANGLES[TRIANGLE_IND][14] = DIFFUSE[2];

            TRIANGLES[TRIANGLE_IND][15] = REFLECTIVE;

            TRIANGLES[TRIANGLE_IND][16] = SPECULAR[0];
            TRIANGLES[TRIANGLE_IND][17] = SPECULAR[1];
            TRIANGLES[TRIANGLE_IND][18] = SPECULAR[2];
            TRIANGLES[TRIANGLE_IND][19] = SPECULAR[3];

            i++;
            TRIANGLE_IND++;
        }
        if (words[0] == "write") return "Reading stage successful.";
    }
    return "Something went wrong when reading file.";
}

#endif // STAGE_INTERPRETER_H
