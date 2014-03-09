#include "MatriceEnnemi.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <sstream>
#include "GUI.h"

using namespace std;

MatriceEnnemi* MatriceEnnemi::s_Instance = NULL;
bool MatriceEnnemi::s_InstanceCree = false;

int MatriceEnnemi::nbrLignes(const std::string& str)
{
    int count = 0;
    bool sameWord = false;
    for(unsigned int i = 0; i < str.length(); i++)
    {
        if (str[i] != '\n' && !sameWord)
        {
            count++;
            sameWord = true;
        }
        else if (str[i] == '\n')
        {
            sameWord = false;
        }
    }

    return count;
}

MatriceEnnemi::MatriceEnnemi(const string& strMatriceEnnemis, int intervalTirEnnemi, int intervalDescenteEnnemi, int nbrTirEnnemiSimultane)
    : m_TimerMouvementEnnemi(20), m_TimerMissiles(intervalTirEnnemi)
{
    m_IntervalTirEnnemi = intervalTirEnnemi;
    m_IntervalDescenteEnnemi = intervalDescenteEnnemi;
    m_NbrTirEnnemiSimultane = nbrTirEnnemiSimultane;

    // ajout des lignes ennemis dans la matrice actuelle
    // on affecte la direction de mouvement (alternative entre les lignes)
    // On calcule et on affecte y de départ pour chaque ligne
    stringstream ss(strMatriceEnnemis);
    string tmp_str;
    int lignes = nbrLignes(strMatriceEnnemis);

    GUI::dessineChargement(lignes + 1);

    LigneEnnemi* ModeleLigne;
    unsigned int count = 0;
    bool direction = true;
    int y_pos = (int)(50 * (GUI::getHeight() / 768.f));
    int height_max;
    for (int i = 0; i < lignes; i++)
    {
        if (count == 5)
            break;
        getline(ss, tmp_str);

        ModeleLigne = new LigneEnnemi(tmp_str, direction, y_pos);
        m_ListeLignesEnnemis.push_back(*ModeleLigne);      // Ajout d'une copie, alors on libère la mémoire just après

        // On cherche la hauteur maximal de la ligne en cours
        // pour que la ligne suivante se positionne juste en dessous
        height_max = 0;
        for (int i = 0; i < ModeleLigne->getTaille(); i++)
        {
            if (height_max < (*ModeleLigne)[i].getRect().h)
                height_max = (*ModeleLigne)[i].getRect().h;
        }

        direction = !direction;
        y_pos += height_max + (int)(20 * (GUI::getHeight() / 768.f));

        delete ModeleLigne;
        count++;

        GUI::dessineChargement();
    }

    m_Taille = count;

    // Initialise le générateur de nombres aléatoires
    srand(time(0));
}

MatriceEnnemi* MatriceEnnemi::getSingleton(const string& strMatriceEnnemis, int intervalTirEnnemi, int intervalDescenteEnnemi, int nbrTirEnnemiSimultane)
{
    if (!s_InstanceCree)
    {
        if (strMatriceEnnemis == "")
        {
            // Erreur, l'instance n'est pas encore crée et on essai de d'appeler getSingleton() sans paramètres
            // Alors on renvoie NULL
            return NULL;
        }
        else
        {
            // On crée l'objet en utilisant les paramètres spécifiés
            s_Instance = new MatriceEnnemi(strMatriceEnnemis, intervalTirEnnemi, intervalDescenteEnnemi, nbrTirEnnemiSimultane);
            s_InstanceCree = true;
            return s_Instance;
        }
    }
    else
    {
        // Dans le cas où on donne des paramètres on les ignore tout simplement car l'objet est déjà crée
        // On renvoie l'unique instance de l'objet
        return s_Instance;
    }
}

void MatriceEnnemi::libere()
{
    if (s_InstanceCree)
        delete s_Instance;
}

MatriceEnnemi::~MatriceEnnemi()
{
    s_InstanceCree = false;
}

LigneEnnemi& MatriceEnnemi::operator[](unsigned int i)
{
    list<LigneEnnemi>::iterator iter = m_ListeLignesEnnemis.begin();
    unsigned int count = 0;
    while (iter != m_ListeLignesEnnemis.end())
    {
        if (count == i)
            return *iter;
        count++;
        iter++;
    }
    iter = m_ListeLignesEnnemis.begin();
    return *iter;
}

LigneEnnemi MatriceEnnemi::operator[](unsigned int i) const
{
    list<LigneEnnemi>::const_iterator iter = m_ListeLignesEnnemis.begin();
    unsigned int count = 0;
    while (iter != m_ListeLignesEnnemis.end())
    {
        if (count == i)
            return *iter;
        count++;
        iter++;
    }
    iter = m_ListeLignesEnnemis.begin();
    return *iter;
}

int MatriceEnnemi::getTailleLignes() const
{
    return m_Taille;
}

void MatriceEnnemi::bougeEnnemis()
{
    if (m_TimerMouvementEnnemi.verifie())
    {
        // On parcours toutes les lignes et on appel la méthode bougeHoriz()
        list<LigneEnnemi>::iterator iter = m_ListeLignesEnnemis.begin();

        while (iter != m_ListeLignesEnnemis.end())
        {
            iter->bougeHoriz();
            iter++;
        }
    }
}

int MatriceEnnemi::getNbrEnnemis()
{
    list<LigneEnnemi>::iterator iter = m_ListeLignesEnnemis.begin();
    int nbr = 0;

    while (iter != m_ListeLignesEnnemis.end())
    {
        nbr += iter->getTaille();
        iter++;
    }

    return nbr;
}

void MatriceEnnemi::tirEnnemis()
{
    if (m_TimerMissiles.verifie())
    {
        // un test pour éviter qu'un ennemi tir plusieurs tir à la fois
        int nbrEnnemis = getNbrEnnemis();
        if (m_NbrTirEnnemiSimultane > nbrEnnemis)
            m_NbrTirEnnemiSimultane = nbrEnnemis;

        for (int i = 0; i < m_NbrTirEnnemiSimultane; i++)
        {
            if (m_Taille > 0)
            {
                int indice = rand() % m_Taille;
                LigneEnnemi* ligne = &((*this)[indice]);
                ligne->tirEnnemis();
            }
        }
    }
}

void MatriceEnnemi::dessineEnnemis(bool dessineSeulement)
{
    list<LigneEnnemi>::iterator iter = m_ListeLignesEnnemis.begin();
    LigneEnnemi* toDelete = NULL;

    while (iter != m_ListeLignesEnnemis.end())
    {
        iter->dessineEnnemis(dessineSeulement);

        if (iter->getTaille() == 0)
        {
            toDelete = &(*iter);
        }

        iter++;

        if (toDelete)
        {
            m_ListeLignesEnnemis.remove(*toDelete);
            m_Taille--;
            toDelete = NULL;
        }
    }
}

//void MatriceEnnemi::redimensionne()
//{
//    list<LigneEnnemi>::iterator iter = m_ListeLignesEnnemis.begin();
//    int height_max;
//    int y_pos = (int)(20 * (GUI::getHeight() / 768.f));
//    while (iter != m_ListeLignesEnnemis.end())
//    {
//        iter->redimensionne();
//        iter->setYDepart(y_pos);
//
//        // On cherche la hauteur maximal de la ligne en cours
//        // pour que la ligne suivante se positionne juste en dessous
//        height_max = 0;
//        for (int i = 0; i < iter->getTaille(); i++)
//        {
//            if (height_max < (*iter)[i].getRect().h)
//                height_max = (*iter)[i].getRect().h;
//        }
//
//        y_pos += height_max + (int)(20 * (GUI::getHeight() / 768.f));
//
//
//
//        iter++;
//    }
//}
