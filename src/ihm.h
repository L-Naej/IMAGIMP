#ifndef IHM_H
#define IHM_H
#include "layer.h"
#include "image.h"

/**
 * Commandes utilisateurs :
 * KEY_LEFT : calque précédent
 * KEY_RIGHT : calque suivant
 * ESCAPE : quitte le programme
 * SPACE : afficher les commandes
 * a: ajouter un calque
 * g: générer image finale
 * c: affiche le layer courant (utile si on est sur l'image finale)
 * v: ajoute un calque vierge
 * o: modifier l'opacité du calque courant
 * m: modifier la fonction de mélange du calque courant
 * d: supprimer le calque courant
 * s: sauvegarde l'image finale
 * h: afficher l'historique
 * u: annuler la dernière opération
 * l: ajouter un effet au calque courant
 * n: supprimer l'effet courant (le dernier de la liste du calque courant
 */

/**
 * Définie les fonctions de callback à appeler
 * lors des interactions utilisateurs clavier et souris.
 */
void initIHM();

/**
 * Analyse la ligne de commande afin de créer
 * le premier calque et ses LUT.
 * Voir sujet du projet section 2.3.4 pour plus d'infos.
 * Renvoie un pointeur vers le calque créé. 
 */
Layer* parseCmdLine(int argc, char** argv);

/**
 * Affiche le calque courant dans la fenêtre
 * graphique.
 */
void displayCurrentLayer(List* layerList);

/**
 * Sécurise l'affichage d'une nouvelle image
 * en testant bien que img n'est pas NULL et
 * contient bien une liste de pixels.
 * (sécurise actualiseImage(unsigned char* img) de 
 * libglimagimp)
 */
void displayImage(const Image* img);

/**
 * Affiche dans le terminal des informations
 * sur le calque courant.
 * Orientée utilisateur contrairement
 * à dumpLayer orienté debug.
 */
void printState();

/**
 * Action utilisateur d'ajout d'un layer
 * vierge.
 */
void userAddEmptyLayer(List* layerList);

/**
 * Action utilisateur d'ajout d'un layer.
 */
void userAddLayer(List* layerList);

/**
 * Affiche en ligne de commande une interface
 * pour demander à l'utilisateur de saisir une 
 * opacité pour le calque courant. 
 * Renvoie l'opacité saisie.
 */
double userSetOpacity();

/**
 * Affiche en ligne de commande une interface
 * pour demander à l'utilisateur de saisir une 
 * opération pour le calque courant. 
 * Renvoie l'opération saisie.
 */
LAYER_OP userSetLayerOp();

/*
 * Supprime le calque courant.
 * Renvoie true si la suppression s'est bien passée,
 * false sinon (on ne peut supprimer un calque
 * s'il est seul).
 */
bool userDelCurrentLayer(List* layerList);

/**
 * Sauvegarde l'image finale, résultat de
 * la succession des calques et de leurs LUTs.
 */
void userSaveFinalImage(List* layerList);

/**
 * Ajoute un lut au calque courant.
 */
bool userAddLut(List* layerList);

/**
 * Fonction de callback appelée lors de l'appui
 * sur une touche "normale" du clavier.
 * @param c le caractère saisi
 * @param x position x de la souris au moment de l'appui
 * @param y position y de la souris au moment de l'appui
 */
void keyboardListener(unsigned char c, int x, int y);

/**
 * Fonction de callback appelée lors de l'appui
 * sur une touche "spéciale" du clavier.
 * @param c la touche spéciale enfoncée (voir http://www.opengl.org/documentation/specs/glut/spec3/node54.html)
 * @param x position x de la souris au moment de l'appui
 * @param y position y de la souris au moment de l'appui
 */
void keyboardSpecialListener(int c, int x, int y);

/**
 * Fonction de callback appelée lors
 * du clic sur la croix de fermeture de la fenêtre,
 * ou lors de l'appuie sur la touche escape.
 * Libère la mémoire avant de fermer le programme.
 */
void exitProgramClean();

/**
 * Affiche dans la console la liste des
 * commandes utilisateur du programme.
 */
void displayCommands();
#endif
