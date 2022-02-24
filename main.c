//
// Jeu d'échecs avec interface graphique GTK
//

#include <gtk/gtk.h>

#include <stdlib.h>

// Pièces blancs
#define BLANC_ROI "♔"
#define BLANC_REINE "♕"
#define BLANC_TOUR "♖"
#define BLANC_FOU "♗"
#define BLANC_CAVALIER "♘"
#define BLANC_PION "♙"

// Pièces noires
#define NOIR_ROI "♚"
#define NOIR_REINE "♛"
#define NOIR_TOUR "♜"
#define NOIR_FOU "♝"
#define NOIR_CAVALIER "♞"
#define NOIR_PION "♟"

// Contexte de l'application
typedef struct _ContexteApp
{
	GtkWidget *label[10][10];

	GtkWidget *deplacer_depuis;
	GtkWidget *deplacer_vers;
} ContexteApp;

//
// Fonction appelée par GTK lors d'une validation de l'utilisateur
//
static void gerer_validation(GtkWidget *objet, ContexteApp *ctx)
{
	// Paramètres non utilisés
	(void)objet;
	(void)ctx;

	// Affiche un message avec les valeurs renseignées pour le déplacement
	printf(
		"L'utilisateur a validé son déplacement deplacer_depuis=%s deplacer_vers=%s\n",
		gtk_entry_get_text(GTK_ENTRY(ctx->deplacer_depuis)),
		gtk_entry_get_text(GTK_ENTRY(ctx->deplacer_vers)));

	// Remet à zéro les entrées
	gtk_entry_set_text(GTK_ENTRY(ctx->deplacer_depuis), "");
	gtk_entry_set_text(GTK_ENTRY(ctx->deplacer_vers), "");
}

//
// Affiche les positions des pièces sur l'échiquier
//
static void echiquier_afficher_positions(ContexteApp *ctx)
{
	for (int i = 1; i < 9; ++i)
	{
		gchar text[2];

		// Renseigne le caractère null de fin de la chaîne de caractères
		text[1] = '\000';

		// Affiche les lettres de 'A' à 'H'
		text[0] = 'A' + i - 1;
		gtk_label_set_text(GTK_LABEL(ctx->label[i][0]), text);
		gtk_label_set_text(GTK_LABEL(ctx->label[i][9]), text);

		// Affiche les nombres de '8' à '1'
		text[0] = '8' - i + 1;
		gtk_label_set_text(GTK_LABEL(ctx->label[0][i]), text);
		gtk_label_set_text(GTK_LABEL(ctx->label[9][i]), text);
	}
}

//
// Affiche toutes les pièces de l'échiquier dans leurs positions initiales
//
static void echiquier_afficher_pieces(ContexteApp *ctx)
{
	for (int i = 1; i < 9; ++i)
	{
		// Affiche les pions
		gtk_label_set_text(GTK_LABEL(ctx->label[i][2]), NOIR_PION);
		gtk_label_set_text(GTK_LABEL(ctx->label[i][7]), BLANC_PION);

		switch (i)
		{
		case 1:
		case 8:
			// Affiche les tours
			gtk_label_set_text(GTK_LABEL(ctx->label[i][1]), NOIR_TOUR);
			gtk_label_set_text(GTK_LABEL(ctx->label[i][8]), BLANC_TOUR);
			break;

		case 2:
		case 7:
			// Affiche les cavaliers
			gtk_label_set_text(GTK_LABEL(ctx->label[i][1]), NOIR_CAVALIER);
			gtk_label_set_text(GTK_LABEL(ctx->label[i][8]), BLANC_CAVALIER);
			break;

		case 3:
		case 6:
			// Affiche les fous
			gtk_label_set_text(GTK_LABEL(ctx->label[i][1]), NOIR_FOU);
			gtk_label_set_text(GTK_LABEL(ctx->label[i][8]), BLANC_FOU);
			break;

		case 4:
			// Affiche les reines
			gtk_label_set_text(GTK_LABEL(ctx->label[i][1]), NOIR_REINE);
			gtk_label_set_text(GTK_LABEL(ctx->label[i][8]), BLANC_REINE);
			break;

		case 5:
			// Affiche les rois
			gtk_label_set_text(GTK_LABEL(ctx->label[i][1]), NOIR_ROI);
			gtk_label_set_text(GTK_LABEL(ctx->label[i][8]), BLANC_ROI);
			break;

		default:
			// Impossible
			break;
		}
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
	gtk_style_context_add_provider_for_screen(
		gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(cssProvider),
		GTK_STYLE_PROVIDER_PRIORITY_USER);

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
				gtk_widget_set_name(ctx->label[colonne][ligne], "case_contour");
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

	// Affiche les positions
	echiquier_afficher_positions(ctx);

	// Affiche les pièces dans leurs positions initiales
	echiquier_afficher_pieces(ctx);

	// Crée une entrée dans laquelle l'utilisateur peut renseigner la position de la pièce à déplacer
	ctx->deplacer_depuis = gtk_entry_new();
	objet = gtk_label_new("pièce en");
	gtk_grid_attach(GTK_GRID(grille), objet, 11, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grille), ctx->deplacer_depuis, 12, 3, 1, 1);

	// Crée une entrée dans laquelle l'utilisateur peut renseigner la position vers laquelle déplacer la pièce
	ctx->deplacer_vers = gtk_entry_new();
	objet = gtk_label_new("vers");
	gtk_grid_attach(GTK_GRID(grille), objet, 11, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grille), ctx->deplacer_vers, 12, 4, 1, 1);

	// Crée un bouton de validation du déplacement
	objet = gtk_button_new();
	gtk_button_set_label(GTK_BUTTON(objet), "Valider");
	g_signal_connect(G_OBJECT(objet), "clicked", G_CALLBACK(gerer_validation), ctx);
	gtk_grid_attach(GTK_GRID(grille), objet, 11, 5, 2, 1);

	// Libération du contexte de l'application
	gtk_widget_show_all(fenetre);
	gtk_main();
	g_slice_free(ContexteApp, ctx);

	return EXIT_SUCCESS;
}
