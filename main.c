
#include <gtk/gtk.h>

#include <stdlib.h>

#define BLANC_ROI "♔"
#define NOIR_ROI "♚"

// Contexte de l'application
typedef struct _ContexteApp
{
	GtkWidget *label[4];
} ContexteApp;

int main(int argc, char **argv)
{
	ContexteApp *ctx;
	GtkWidget *fenetre;
	GtkWidget *grille;

	gtk_init(&argc, &argv);

	ctx = g_slice_new(ContexteApp);

	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
	gtk_style_context_add_provider_for_screen(
		gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(cssProvider),
		GTK_STYLE_PROVIDER_PRIORITY_USER);

	fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(fenetre), "Jeu d'échecs");
	g_signal_connect(G_OBJECT(fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	grille = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(fenetre), grille);

	ctx->label[0] = gtk_label_new(BLANC_ROI);
	gtk_widget_set_name(GTK_WIDGET(ctx->label[0]), "case_gris");
	gtk_grid_attach(GTK_GRID(grille), ctx->label[0], 0, 0, 1, 1);

	ctx->label[1] = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grille), ctx->label[1], 0, 1, 1, 1);

	ctx->label[2] = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grille), ctx->label[2], 1, 0, 1, 1);

	ctx->label[3] = gtk_label_new(NOIR_ROI);
	gtk_widget_set_name(GTK_WIDGET(ctx->label[3]), "case_gris");
	gtk_grid_attach(GTK_GRID(grille), ctx->label[3], 1, 1, 1, 1);

	gtk_widget_show_all(fenetre);
	gtk_main();
	g_slice_free(ContexteApp, ctx);

	return EXIT_SUCCESS;
}
