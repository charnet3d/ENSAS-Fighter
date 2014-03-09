#include "LigneEnnemi.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <sstream>
#include "GUI.h"

using namespace std;

int LigneEnnemi::nbrMots(const std::string& str)
{
    int count = 0;
    bool sameWord = false;
    for(unsigned int i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && !sameWord)
        {
            count++;
            sameWord = true;
        }
        else if (str[i] == ' ' || str[i] == '\t')
        {
            sameWord = false;
        }
    }

    return count;
}

LigneEnnemi::LigneEnnemi(const std::string& strLigneEnnemis, bool directionHoriz, int yDepart)
{
    m_YDepart = yDepart;
    m_DirectionHoriz = directionHoriz;

    // ajout des ennemis selon leurs types dans le stage actuel
    stringstream ss(strLigneEnnemis);
    string tmp_str;
    int mots = nbrMots(strLigneEnnemis);
    Ennemi* ModeleEnnemi;
    int count = 0;
    int taille = 0;
    int height_max = 0;
    for (int i = 0; i < mots; i++)
    {
        if (count == 10)
            break;
        ss >> tmp_str;

        ModeleEnnemi = new Ennemi(0, 0, tmp_str);
        m_ListeEnnemis.push_back(*ModeleEnnemi);      // Ajout d'une copie, alors on libère la mémoire just après

        // on calcule la largeur total de la ligne d'ennemis en pixels
        taille += ModeleEnnemi->getRect().w;

        if (height_max < ModeleEnnemi->getRect().h)
            height_max = ModeleEnnemi->getRect().h;

        delete ModeleEnnemi;

        count++;
    }

    // ajout de l'espace de séparation entre les ennemis
    taille += (m_ListeEnnemis.size() - 1) * (int)(30 * (GUI::getWidth() / 1024.f));
    m_Taille = count;

    // modification des coordonnées de départ pour les ennemis
    int tmp_x = (GUI::getWidth() - taille) / 2;
    list<Ennemi>::iterator iter = m_ListeEnnemis.begin();
    while (iter != m_ListeEnnemis.end())
    {
        // Les ennemis doivent être centré entre eux
        iter->setPos(tmp_x, m_YDepart + (height_max - iter->getRect().h) / 2);

        tmp_x += iter->getRect().w + (int)(30 * (GUI::getWidth() / 1024.f));

        iter++;
    }

    // Initialise le générateur de nombres aléatoires
    srand(time(0));
}

Ennemi& LigneEnnemi::operator[](unsigned int i)
{
    list<Ennemi>::iterator iter = m_ListeEnnemis.begin();
    unsigned int count = 0;
    while (iter != m_ListeEnnemis.end())
    {
        if (count == i)
            return *iter;
        count++;
        iter++;
    }
    iter = m_ListeEnnemis.begin();
    return *iter;
}

Ennemi LigneEnnemi::operator[](unsigned int i) const
{
    list<Ennemi>::const_iterator iter = m_ListeEnnemis.begin();
    unsigned int count = 0;
    while (iter != m_ListeEnnemis.end())
    {
        if (count == i)
            return *iter;
        count++;
        iter++;
    }
    iter = m_ListeEnnemis.begin();
    return *iter;
}

bool LigneEnnemi::operator==(const LigneEnnemi& le) const
{
    if (this == &le)
        return true;
    else
        return false;
}

int LigneEnnemi::getTaille() const
{
    return m_Taille;
}

//void LigneEnnemi::setYDepart(int yDepart)
//{
//    m_YDepart = yDepart;
//
//    // modification des coordonnées de départ pour les ennemis
//    list<Ennemi>::iterator iter = m_ListeEnnemis.begin();
//    int height_max = 0;
//
//    while (iter != m_ListeEnnemis.end())
//    {
//        if (height_max < iter->getRect().h)
//            height_max = iter->getRect().h;
//        iter++;
//    }
//
//    iter = m_ListeEnnemis.begin();
//    while (iter != m_ListeEnnemis.end())
//    {
//        // Les ennemis doivent être centré entre eux
//        iter->setPos(iter->getRect().x, m_YDepart + (height_max - iter->getRect().h) / 2);
//        iter++;
//    }
//}

void LigneEnnemi::descendre()
{

}

void LigneEnnemi::bougeHoriz()
{
    Ennemi *dern_ennemi = &((*this)[m_Taille - 1]); // pointeur sur le dernier ennemi de la ligne
    Ennemi *prem_ennemi = &((*this)[0]);    // pointeur sur le premier ennemi de la ligne
    int vitesse = 0;

    if (m_DirectionHoriz)
        vitesse = 1;
    else
        vitesse = -1;

    list<Ennemi>::iterator iter = m_ListeEnnemis.begin();

    while (iter != m_ListeEnnemis.end())
    {
        // On le bouge selon une vitesse qui va determiner aussi la direction
        iter->setPos(iter->getRect().x + vitesse, iter->getRect().y);

        // On choisi le dernier ennemi, et on teste sur l'arrivée au bord droit de l'ecran
        if ((dern_ennemi->getRect().x + dern_ennemi->getRect().w) >= GUI::getWidth() - (int)(20 * GUI::getWidth() / 1024.f))
            m_DirectionHoriz = false;
            // Sinon on teste sur l'arrivée au bord gauche pour le premier ennemi
        else if (prem_ennemi->getRect().x <= (int)(20 * GUI::getWidth() / 1024.f))
            m_DirectionHoriz = true;

        iter++;
    }
}

void LigneEnnemi::tirEnnemis()
{
    int indice = rand() % (getTaille());
    Ennemi* e = &((*this)[indice]);
    e->attaque();
}

void LigneEnnemi::dessineEnnemis(bool dessineSeulement)
{
    list<Ennemi>::iterator iter = m_ListeEnnemis.begin();
    Ennemi* toDelete = NULL;

    while (iter != m_ListeEnnemis.end())
    {
        iter->dessineEntite(dessineSeulement);

        if (!(iter->getNiveauVie()))
        {
            toDelete = &(*iter);
        }

        iter++;

        if (toDelete)
        {
            m_ListeEnnemis.remove(*toDelete);
            m_Taille--;
            toDelete = NULL;
        }
    }
}

//void LigneEnnemi::redimensionne()
//{
//    list<Ennemi>::iterator iter = m_ListeEnnemis.begin();
//    std::vector<int> vX, vW;
//    int i = 0;
//
//    while (iter != m_ListeEnnemis.end())
//    {
//        vX.push_back(iter->getRect().x);
//        vW.push_back(iter->getRect().w);
//        std::cout << vX[i] << " " << vW[i] << std::endl;
//
//        iter++;
//        i++;
//    }
//
//    // Calcule de l'espace séparant les ennemis avant redimensionnement
//    // d'abord on calcule la taille totale des ennemis sans espaces entre eux
//    int taille = 0;
//    for (i = 0; i < (int)(vW.size()); i++)
//        taille += vW[i];
//
//    // Après on calucule l'espace de séparation entre les ennemis
//    int tailleAvecSeparation = (vX[m_Taille - 1] + vW[m_Taille - 1]) - vX[0];
//    int ancienSeparateur = (tailleAvecSeparation - taille) / (m_Taille - 1);
//
//    iter = m_ListeEnnemis.begin();
//    taille = 0;
//    while (iter != m_ListeEnnemis.end())
//    {
//        iter->redimensionne(true);
//        // mise à jour de la taille totale après redimensionnement
//        taille += iter->getRect().w;
//        iter++;
//    }
//
//    // Calcule de la nouvelle taille avec séparation
//    tailleAvecSeparation = taille + (m_Taille - 1) * (int)(30 * (GUI::getWidth() / 1024.f));
//
//    iter = m_ListeEnnemis.begin();
//    // modification des coordonnées de départ pour les ennemis
//    if (vX.size() > 0)
//    {
//        int tmp_x = (int)(vX[0] * (GUI::getWidth() / 1024.f));
//        //int tmp_x = (GUI::getWidth() - tailleAvecSeparation) / 2;
//        int diffPos;
//        Ennemi* precedent;
//        i = 1;
//        while (iter != m_ListeEnnemis.end())
//        {
//            iter->setPos(tmp_x, iter->getRect().y);
//            precedent = &(*iter);
//
//            iter++;
//            if (vX.size() > 1)
//                diffPos = (vX[i] - ancienSeparateur) - (vX[i - 1] + vW[i - 1]);
//
//            tmp_x += precedent->getRect().w + diffPos + (int)(30 * (GUI::getWidth() / 1024.f));
//        }
//    }
////
////        iter = m_ListeEnnemis.begin();
////        int i = 0;
////        while (iter != m_ListeEnnemis.end())
////        {
////            std::cout << iter->getRect().x;
////            iter->setPos(vX[i], iter->getRect().y);
////            std::cout << " " << iter->getRect().x << std::endl;
////            //std::cout << vX[i] << std::endl;
////            iter++;
////            i++;
////        }
////    }
//}
