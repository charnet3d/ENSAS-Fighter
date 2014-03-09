#include "EntreeSortie.h"
#include "GUI.h"

int main(int argc, char* argv[])
{
    // Pour que les 'cout' fonctionnent normalement:
    // Sans ces instructions un cout s'écrit automatiquement dans le fichier "stdout.txt"
    // freopen("CONOUT$", "wt", stdout);
    // freopen("CONOUT$", "wt", stderr);

    EntreeSortie::initialiseSDL();

    bool fin = false;

    while (!fin)
    {
        switch (GUI::s_InterfaceActuelle)
        {
            case TITRE:
                GUI::Titre();
                break;
            case MENU_PRINCIPALE:
                GUI::MenuPrincipal();
                break;
            case EN_JEU:
                GUI::ChoixStage();
                break;
            case FIN_JEU:
                GUI::FinJeu();
                break;
            case COMMENT_JOUER:
                GUI::CommentJouer();
                break;
            case OPTIONS:
                GUI::MenuOptions();
                break;
            case QUITTER:
                fin = true;
                break;
        }
    }

    EntreeSortie::libere();
    return EXIT_SUCCESS;
}
