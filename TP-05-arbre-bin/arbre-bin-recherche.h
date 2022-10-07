#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef int Element;

struct noeud_s {
	Element val;
	struct noeud_s* filsGauche;
	struct noeud_s* filsDroit;
};

typedef struct noeud_s Noeud;
typedef Noeud* ArbreBinaire;

// retourne TRUE si a est l'arbre vide et FALSE sinon
bool estVide(ArbreBinaire a);

// initialise l'arbre a à l'arbre vide
void initialiser(ArbreBinaire* a);

// créer un arbre contenant un seul noeud ayant la valeur e
ArbreBinaire creer(Element e);

// insere e dans a sachant que a est un arbre binaire de recherche
// si a contient déjà un element e, ce dernier n'est pas insérer afin d'éviter les doublons
// version itérative
ArbreBinaire insere_i(ArbreBinaire a, Element e);

// insere e dans a sachant que a est un arbre binaire de recherche
// si a contient déjà un element e, ce dernier n'est pas insérer afin d'éviter les doublons
// version récursive
ArbreBinaire insere_r(ArbreBinaire a, Element e);

// retourne le nombre de noeud contenus dans l'arbre a
int nombreDeNoeud(ArbreBinaire a);

// retourne la profondeur du noeud ayant la valeur e dans a
// retourne -1 si a est vide ou si e n'est pas dans a
int profondeur(ArbreBinaire a, Element e);

// retourne la hauteur de l'arbre a
int hauteur(ArbreBinaire a);

// retourne le pere de elem dans l'arbre a ou NULL s'il n'existe pas
ArbreBinaire pere(ArbreBinaire a, Element elem);

void afficheRGD_r(ArbreBinaire a);
void afficheGRD_r(ArbreBinaire a);
void afficheGDR_r(ArbreBinaire a);

// retourne le noeud dont la valeur est minimum dans l'arbre
// Suppose que a est un arbre binaire de recherche sans doublons
ArbreBinaire min(ArbreBinaire a);

// retourne le noeud dont la valeur est maximum dans l'arbre
// Suppose que a est un arbre binaire de recherche sans doublons
ArbreBinaire max(ArbreBinaire a);

// retourne l'arbre dont la valeur de la racine est elem et NULL si elem n'existe dans a
// version récursive
ArbreBinaire recherche_r(ArbreBinaire a, Element elem);

// suppime x de a
ArbreBinaire supprimer_r(ArbreBinaire a,Element x);

void detruire_r(ArbreBinaire a);

