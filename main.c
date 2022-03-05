//
// Jeu d'échecs avec interface graphique GTK
//

#include <gtk/gtk.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BLANC_ROI "♔"
#define NOIR_ROI "♚"

#define BLANC_REINE "♕"
#define NOIR_REINE "♛"

#define NOIR_TOUR "♜"
#define BLANC_TOUR "♖"

#define BLANC_PION "♙"
#define NOIR_PION "♟︎"

#define NOIR_FOU "♝"
#define BLANC_FOU "♗"

#define NOIR_CAVALIER "♞"
#define BLANC_CAVALIER "♘"

// Contexte de l'application
typedef struct
{
	GtkWidget *label[10][10]; // Stocke les coordonées des cases
	GtkWidget *source;
	GtkWidget *destination;
} ContexteApp;

//
// Fonction appelée par GTK lors d'une validation de l'utilisateur
//
static void validation(GtkWidget *objet, ContexteApp *ctx)
{
	const gchar *source;
	const gchar *destination;
	int colonne;
	int ligne;
	int colonne1;
	int ligne1;
	const gchar *stock;
	gchar *stock1;

	// Paramètres non utilisés
	(void)objet;
	(void)ctx;

	source = gtk_entry_get_text(GTK_ENTRY(ctx->source));
	destination = gtk_entry_get_text(GTK_ENTRY(ctx->destination));

	if ((strlen(source) != 2 || (strlen(destination) != 2)))
	{
		printf("COORDONNEE NON VALIDES !!! ");
		exit(-1);
	}

	colonne = source[0] - 'A' + 1;
	ligne = 8 - (source[1] - '1');

	colonne1 = destination[0] - 'A' + 1;
	ligne1 = 8 - (destination[1] - '1');

	// Affiche un message avec les valeurs renseignées pour le déplacement
	printf(
		"L'utilisateur a validé son déplacement source=%s destination=%s\n",
		gtk_entry_get_text(GTK_ENTRY(ctx->source)),
		gtk_entry_get_text(GTK_ENTRY(ctx->destination)));

	stock = gtk_label_get_text(GTK_LABEL(ctx->label[colonne][ligne]));
	stock1 = strdup(stock);

	gtk_label_set_text(GTK_LABEL(ctx->label[colonne][ligne]), "");
	gtk_label_set_text(GTK_LABEL(ctx->label[colonne1][ligne1]), stock1);

	// Remet à zéro les entrées
	gtk_entry_set_text(GTK_ENTRY(ctx->source), "");
	gtk_entry_set_text(GTK_ENTRY(ctx->destination), "");

	free(stock1);
}

// Affiche la position des pieces
static void pieces_position(ContexteApp *ctx)
{
	for (int i = 1; i <= 8; ++i)
	{
		gchar text[2];

		// Renseigne le caractère null de fin de la chaîne de caractères
		text[1] = '\000';

		// Affiche les lettres de 'A' à 'H'
		text[0] = 'A' + i - 1;
		gtk_label_set_text(GTK_LABEL(ctx->label[i][9]), text);

		// Affiche les nombres de '8' à '1'
		text[0] = '8' - i + 1;
		gtk_label_set_text(GTK_LABEL(ctx->label[0][i]), text);
	}
}

//
// Point d'entrée du programme
//
int main(int argc, char **argv)
{
	ContexteApp *ctx;
	GtkWidget *fenetre;
	GtkWidget *grille;
	GtkWidget *objet;

	// Initialisation de la librairie GTK
	gtk_init(&argc, &argv);

	// Allocation du contexte de l'application
	ctx = g_slice_new(ContexteApp);

	// Chargement du fichier de style
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	// Création la fenêtre principale
	fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(fenetre), "Jeu d'échecs");
	g_signal_connect(G_OBJECT(fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Ajout de la grille dans la fenêtre
	grille = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(fenetre), grille);

	// Création d'un échiquier vide
	for (int colonne = 0; colonne < 10; ++colonne)
	{
		for (int ligne = 0; ligne < 10; ++ligne)
		{
			ctx->label[colonne][ligne] = gtk_label_new("");
			gtk_grid_attach(GTK_GRID(grille), ctx->label[colonne][ligne], colonne, ligne, 1, 1);

			// Force la taille du widget pour que l'affichage ne dépende pas du texte
			gtk_widget_set_size_request(ctx->label[colonne][ligne], 96, 96);

			// Fixe le style de la case
			// On distingue une case sur deux pour l'échiquier ainsi que le contour
			if ((ligne == 0) || (ligne == 9) || (colonne == 0) || (colonne == 9))
			{
				gtk_widget_set_name(ctx->label[colonne][ligne], "case_noire");
			}
			else if (((colonne + ligne) % 2) == 0)
			{
				gtk_widget_set_name(ctx->label[colonne][ligne], "case_beige");
			}
			else
			{
				gtk_widget_set_name(ctx->label[colonne][ligne], "case_gris");
			}
		}
	}

	// Positionnement des pièces
	gtk_label_set_text(GTK_LABEL(ctx->label[5][1]), NOIR_ROI);
	gtk_label_set_text(GTK_LABEL(ctx->label[5][8]), BLANC_ROI);

	gtk_label_set_text(GTK_LABEL(ctx->label[4][8]), BLANC_REINE);
	gtk_label_set_text(GTK_LABEL(ctx->label[4][1]), NOIR_REINE);

	gtk_label_set_text(GTK_LABEL(ctx->label[1][1]), NOIR_TOUR);
	gtk_label_set_text(GTK_LABEL(ctx->label[8][1]), NOIR_TOUR);

	gtk_label_set_text(GTK_LABEL(ctx->label[1][8]), BLANC_TOUR);
	gtk_label_set_text(GTK_LABEL(ctx->label[8][8]), BLANC_TOUR);

	gtk_label_set_text(GTK_LABEL(ctx->label[3][1]), NOIR_FOU);
	gtk_label_set_text(GTK_LABEL(ctx->label[6][1]), NOIR_FOU);
	gtk_label_set_text(GTK_LABEL(ctx->label[3][8]), BLANC_FOU);
	gtk_label_set_text(GTK_LABEL(ctx->label[6][8]), BLANC_FOU);

	gtk_label_set_text(GTK_LABEL(ctx->label[2][1]), NOIR_CAVALIER);
	gtk_label_set_text(GTK_LABEL(ctx->label[7][1]), NOIR_CAVALIER);
	gtk_label_set_text(GTK_LABEL(ctx->label[2][8]), BLANC_CAVALIER);
	gtk_label_set_text(GTK_LABEL(ctx->label[7][8]), BLANC_CAVALIER);

	for (int i = 1; i <= 8; ++i)
	{

		gtk_label_set_text(GTK_LABEL(ctx->label[i][2]), NOIR_PION);
	}

	for (int i = 1; i <= 8; ++i)
	{
		gtk_label_set_text(GTK_LABEL(ctx->label[i][7]), BLANC_PION);
	}

	pieces_position(ctx);

	// Crée une entrée dans laquelle l'utilisateur peut renseigner la position de la pièce à déplacer
	ctx->source = gtk_entry_new();
	objet = gtk_label_new("Déplacer de : ");
	gtk_grid_attach(GTK_GRID(grille), objet, 11, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grille), ctx->source, 12, 3, 1, 1);

	// Crée une entrée dans laquelle l'utilisateur peut renseigner la position vers laquelle déplacer la pièce
	ctx->destination = gtk_entry_new();
	objet = gtk_label_new("Vers : ");
	gtk_grid_attach(GTK_GRID(grille), objet, 11, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grille), ctx->destination, 12, 4, 1, 1);

	// Crée un bouton de validation du déplacement
	objet = gtk_button_new();
	gtk_button_set_label(GTK_BUTTON(objet), "Valider");
	g_signal_connect(G_OBJECT(objet), "clicked", G_CALLBACK(validation), ctx);
	gtk_grid_attach(GTK_GRID(grille), objet, 12, 5, 1, 1);

	gtk_widget_show_all(fenetre);
	gtk_main();

	// Libération du contexte de l'application
	g_slice_free(ContexteApp, ctx);

	return EXIT_SUCCESS;
}
