#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "image.h"
#include "../make_solve/resolution/send_solution.h"
#include "ocrmnist2.h"



GtkWidget *window;
gint window_width,window_height;
GtkWidget *grid ;
GtkWidget *button_box;
char *path_image;
GtkWidget * new_grid;
gchar *path_screenshot = "../make_solve/Images_mnist/screenshot0.bmp";


typedef struct {
    gdouble red;
    gdouble green;
    gdouble blue;
}
RGBColor;

int main(int argc, char *argv[]); 


// --------------------------------------  BACKGROUND TRAITEMENT ---------------------------------

RGBColor count_pixel_colors(GdkPixbuf *pixbuf, int x, int y, int width, int height) {

    // Cette fonction pour compter la fréquence des couleurs dans une région rectangulaire de l'image
    // Utile pour recuperer le background 

    gint channels = gdk_pixbuf_get_n_channels(pixbuf);
    // on recupere le nombre de canaux de l'image (typiquement 3 pour RGB : Rouge, Vert, Bleu)
    gint rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

    RGBColor totalColor = {0, 0, 0};
    gint count = 0;

    for (int j = y; j < y + height; j++) {
        for (int i = x * channels; i < (x + width) * channels; i += channels) {
            totalColor.red += pixels[j * rowstride + i];
            totalColor.green += pixels[j * rowstride + i + 1];
            totalColor.blue += pixels[j * rowstride + i + 2];
            count++;
        }
    }

    // On calcule la moyenne pour maximisé la reussite
    totalColor.red /= count;
    totalColor.green /= count;
    totalColor.blue /= count;

    return totalColor;
}

RGBColor detect_background_color(const gchar *filename, int x, int y) {
    // Cette fonction permet de detecter l'arriere plan d'une image

    RGBColor background_color = {0, 0, 0};


    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, &error);

    if (error != NULL)
    {
        return background_color;
    }

    // On prend la region superieur gauche car generalement il y a pas de galere la bas
    background_color = count_pixel_colors(pixbuf, 0, 0, x, y);

    g_object_unref(pixbuf);  // on libere la memoire

    return background_color;
}
// Buttons Backgound 
void remove_last_char(char *str) {
    if (str == NULL || str[0] == '\0') {
        return;
    }

    size_t len = strlen(str);
    if (len > 1) {
        str[len - 1] = '\0';
    } else {
        str[0] = '\0';
    }
}


//Magie noir
char *gdk_rgba_to_hex(GdkRGBA *color) {
    // Assurez-vous que les composantes de couleur sont dans la plage valide (0 à
    // 1)
    double red = (color->red > 1.0) ? 1.0 : ((color->red < 0) ? 0 : color->red);
    double green =
        (color->green > 1.0) ? 1.0 : ((color->green < 0) ? 0 : color->green);
    double blue =
        (color->blue > 1.0) ? 1.0 : ((color->blue < 0) ? 0 : color->blue);
    double alpha =
        (color->alpha > 1.0) ? 1.0 : ((color->alpha < 0) ? 0 : color->alpha);

    // Convertir les composantes de couleur en valeurs hexadécimales
    int red_int = (int)(red * 255);
    int green_int = (int)(green * 255);
    int blue_int = (int)(blue * 255);
    int alpha_int = (int)(alpha * 255);

    // Allouer de l'espace pour le str (9 caracteres + '\0')
    char *hex_color = calloc(9,1);

    // Formater la chaîne hexadécimale
    snprintf(hex_color, 9, "#%02X%02X%02X%02X", red_int, green_int, blue_int,
            alpha_int);

    return hex_color;
}

void apply_button_color(GtkWidget *widget, GdkRGBA *color) {
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    char *hex_color = gdk_rgba_to_hex(color);
    remove_last_char(hex_color);
    // Ajout pour le débogage

    const char *css_data_template =
        "* { background-image:none; background-color:%s;}";
    char css_data[150];
    snprintf(css_data, sizeof(css_data), css_data_template, hex_color);

    gtk_css_provider_load_from_data(cssProvider, css_data, -1, NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(cssProvider);

    free(hex_color);
}

void apply_color_to_buttons(GdkRGBA *color) {
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(button_box));

    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        GtkWidget *child = GTK_WIDGET(iter->data);
        apply_button_color(child, color);
    }

    g_list_free(children);
}


int tr = 0;

void  apply_back_ground_color(char * filename)
{
    //Application de la couleur sur l'arriere plan 


    RGBColor background_color = detect_background_color(filename, 10, 10);
    // On detecte la couleur moyenne dans le coin en haut a gauche docn du carre
    // (0,0) --> (10,10)
    GdkRGBA color = {
        background_color.red / 255.0,
        background_color.green / 255.0,
        background_color.blue / 255.0,
        1.0
    }; // On cree la couleur associe au background


    //--- couleur des boutons ---

    if (tr == 1)
    {
        GdkRGBA color2 = {fmod((background_color.red + 255), 256) / 255.0,
            fmod((background_color.green + 215), 256) / 255.0,
            fmod((background_color.blue + 50), 256) / 255.0, 1.0};
        apply_color_to_buttons(&color2);}
    //-------- /// ---------

    apply_button_color(window,&color);
    // On modif l'arriere plan dynamiquement
}

// -------------------------------------- IMAGE TRAITEMENT --------------------------------------

int is_square(int w,int h)
{
    double res ;
    if (w > h)
    {
        res = (double)h / (double)w;
    }
    else if (h > w)
    {
        res = (double)w / (double)h;
    }
    else
    {
        return 1;
    }
    if (res > 0.75)  // C'est une carre 
    {
        return 1;
    }
    return 0;
}



void save_image(GtkWidget *image) {
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

    if (pixbuf != NULL) {
        gchar *folder_path =  "testImageJules/"; // Remplacez cela par le chemin de votre dossier
        gchar *image_path = g_build_filename(folder_path, path_image, NULL);
        gdk_pixbuf_save(pixbuf, image_path, "png", NULL, NULL);
        g_free(image_path);
    } 
}

GtkWidget*  load_image_from_path_and_adjust_resolution(char* path)
{
    GError* error = NULL;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(path, &error);

    int width,height;
    width = gdk_pixbuf_get_width(pixbuf);
    height = gdk_pixbuf_get_height(pixbuf);

    double scaleW = ((double)width)/((double)window_width);
    double scaleH = ((double)(height))/((double)window_height);
    double x = 0.2;
    double y = 0.2;

    int square = is_square(width, height);


    if (square == 0)
    {
        if (x + scaleW <0.6)
        {
            x += 0.2;
        }
        if (x + scaleW> 1 )
        {
            while (x+scaleW > 0.8)
            {
                width -= 100;
                scaleW = ((double)width)/((double)window_width);
            }
            x = 0.3;
        }
        if (y + scaleH < 0.6)
        {
            y+=0.2;
        }
        else if  (y + scaleH > 0.8 ) 
        {
            while (y+scaleH > 0.8)
            {
                height -= 100;
                scaleH = ((double)height)/((double)window_height);
            }
            y = 0.3;


        }

    }
    else

    {
        if (scaleH > 0.8 || scaleW > 0.8)
        {
            while (scaleH > 0.8 || scaleW > 0.8)
            {
                height/= 1.5;
                width/=1.5;
                scaleH = (double)height / (double)window_height;
                scaleW = (double)width / (double) window_width; 
            }
        }   
    }
    double w = ((scaleW + x)*((double)window_width)) - 200;
    double h = ((scaleH + y)*((double)window_height)) - 100;
    pixbuf = gdk_pixbuf_scale_simple(pixbuf,(int)w , (int)h, GDK_INTERP_BILINEAR);

    GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);
    return image;
}

void adapt_image_resolution(GtkWidget *image, GtkWidget *window)
{
    // On modifie la resolution de l'image pour l'adapter au cadre
    gint window_width, window_height;
    gtk_window_get_size(GTK_WINDOW(window), &window_width, &window_height); 
    gtk_widget_set_size_request(image, window_width/4, window_height/4);
    // techniquement l'image est dans un petit carre donc la taille de la window est trop grande
}

void get_image_dimensions(const gchar *filename, int *width, int *height) {
    // Cette fonction recupere la dimension d'une image
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    // Concretement cela stock l'image dans une variable 

    if (error != NULL) {
        g_error_free(error);
        return;
    }

    *width = gdk_pixbuf_get_width(pixbuf);
    *height = gdk_pixbuf_get_height(pixbuf);
    g_object_unref(pixbuf); // = free()
}

void delete_files_with_path(const char *image_path) {

    // Supprimer le fichier
    if (remove(image_path) == 0) {
        g_print("L'image a été supprimée avec succès.\n");
    } else 
    {
        g_print("Erreur lors de la suppression de l'image.\n");
    }
}



// -------------------------------------- IMAGE SWITCH --------------------------------------



void replace_image(GtkWidget *grid,char *path, GtkWidget *window  )
{
    int image_width, image_height;
    get_image_dimensions(path, &image_width, &image_height);

    GtkWidget *new_image = load_image_from_path_and_adjust_resolution(path); // on cree une image

    // on recup l'ancienne image à l'aide de la grille
    GtkWidget *old_image = gtk_grid_get_child_at(GTK_GRID(grid), 2, 1);

    gtk_container_remove(GTK_CONTAINER(grid), old_image); // on delete l'ancienne
    path_image = path; // update le path
    gtk_grid_attach(GTK_GRID(grid), new_image, 2, 1, 1, 1);    // et ajoute la nouvelle
                                                               //gtk_window_resize(GTK_WINDOW(window), image_width + 100 , image_height + 100);
                                                               // on ajuste la fenetre pour l'adapter a la resolution de la nouvelle image
    apply_back_ground_color(path);

    gtk_widget_set_hexpand(new_image, TRUE);
    gtk_widget_set_vexpand(new_image, TRUE);
    //Change la background

    // BONUS : Changer la couleur des boutons ...

    gtk_widget_show_all(window);
}





//------------------------------------------------------------------------------------------------------------------------------ IMG RELATED STUFF ----------------------------------------//
#define QUEUE_MAX 30 
static char *starter = "image.jpg";
char path[11] = "step__.png";
int step = -1;
int solved = 0;
SDL_Surface *img = NULL;

// STATIC QUEUE
char *Q[QUEUE_MAX];
int len = -1;
int cpt = 65;
int angle = 0;
int rotated = 0;

void enqueue(char **Q, char *el)
{
    replace_image(grid,el,window);
    ++len;
    Q[len] = g_strdup(el);
    img = load_image(el);
    for(int i = 0; i <= len; i++)
        printf("%s ",Q[i]);
    printf("\n");
}
void dequeue(char **Q)
{
    if(len == 0)
    {
        Q[len--] = 0;
        img = load_image(starter);
        replace_image(grid,starter,window);
    }
    else if(len > 0)
    {
        Q[len] = 0;
        --len;
        //HANDLING THE STEP BY STEP IMAGES
        if(Q[len][0] == 's' && Q[len][1] == 't')
            step--;
        img = load_image(Q[len]);
        replace_image(grid,Q[len],window);
        for(int i = 0; i <= len; i++)
            printf("%s ",Q[i]);
        printf("\n");
    }
}
void reset(char * el)
{
    for(;len >= 0; --len)
        Q[len] = 0;	
    starter = NULL;
    starter = g_strdup(el);
    img = load_image(el);
    replace_image(grid,el,window);
    rotated = solved = 0;
    step = -1;
    printf("starter %s \n",starter);
}
void enqueue_img()
{
    // SAVING IMAGE PROCESS
    // NAMES : A.png, B.png, C.png, (...)
    char * name;
    name = calloc(6,1);
    name[0] = cpt;
    strcat(name,".png");
    IMG_SavePNG(img,name);
    ++cpt;
    enqueue(Q,name);
    free(name);
}
void enqueue_grid()
{
    ++cpt;
    enqueue(Q,"grid.png");
}
void auto_turn()
{
    if(rotated == 0)
    {
        int w = img->w;
        int h = img->h;
        SDL_Surface *cpy = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
        SDL_BlitSurface(img,NULL,cpy,NULL);
        //surface_to_adaptive_treshold(cpy,2);
        speed_ada(cpy,21);
        print_grid(cpy);
        surface_to_hough(cpy,&angle);
        SDL_FreeSurface(cpy);
        rotated++;
        surface_to_rotate_shear(&img,angle);
        enqueue_img();
    }
}
void new_step()
{
    if(step == -1)
        enqueue(Q,"empty_grid.png");
    else
    {
        path[4] = step / 10 + '0';
        path[5] = step % 10 + '0';
        enqueue(Q,path);
    }
    step++;
}
// --------------------------------------- CSS INJECTION ----------------------------------------
void load_css (void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
    const gchar *css_style_file = "main.css"; //fichier reference CSS 
    GFile *css_fp = g_file_new_for_path(css_style_file);
    GError *error = 0;
    // Load le CSS
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display); 
    //****
    // Injection de CSS
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_fp, &error); 
    g_object_unref(provider); // On libere la memoire 
}

// -------------------------------------- FILE SELECTOR --------------------------------------


void file_selected() {

    // On utilise GTK.FileChooserDialog pour ouvrir l'explorateur de fichier

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Sélectionnez un fichier",
            NULL,
            action,
            "_Annuler",
            GTK_RESPONSE_CANCEL,
            "_Ouvrir",
            GTK_RESPONSE_ACCEPT,
            NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) { // Si l'user a clique sur un fichier
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        g_print ("Fichier sélectionné : %s\n", filename); // TEST

        const char *extension = strrchr(filename, '.');//on cherche tout les fichiers avec l'extension .jpg ou .png
        if (extension != NULL) {
            if (strcasecmp(extension, ".jpg") == 0 || strcasecmp(extension, ".jpeg") == 0) 
            {
                g_print("Le fichier est au format JPG.\n");// TEST
                path_image = filename;				
                reset(filename);

            }
            else if (strcasecmp(extension, ".png") == 0) 
            {
                g_print("Le fichier est au format PNG.\n");// TEST
                reset(filename);
                path_image = filename;
            }
            else {
                g_print("Le fichier n'est pas au format JPG ou PNG.\n"); // TEST
            }
        } 
        else {
            g_print("L'extension du fichier n'a pas été trouvée.\n"); // TEST
        }
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

// -------------------------------------- DRAWING AREA --------------------------------------
cairo_surface_t *surface = NULL;
double previous_x = -1;
double previous_y = -1;
int iter = 0;
GtkWidget* chiffre_box;
int current_pred = -1;
GtkWidget* vide; 
GtkWidget* show_text_bon_numeros;
GtkWidget* titre;
GtkWidget* box_yes_no;
GtkWidget* box;
GtkWidget* show_text;
GtkWidget* frame;
GtkWidget* label_text;
GtkWidget * drawing_area;


GtkWidget* clear_button;

int yes = 1;
int clear = 0;
int widget_show = 0;



void apply_color_to_number(GdkRGBA *color, int num) {
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(chiffre_box));
    int result;
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {

        GtkWidget *child = GTK_WIDGET(iter->data);
        result = atoi(gtk_button_get_label(GTK_BUTTON(child)));
        if (result == num)
        {
            apply_button_color(child, color);
        }

    }

    g_list_free(children);
}
void close_menu()
{
    gtk_container_remove(GTK_CONTAINER(new_grid),show_text_bon_numeros);
    gtk_container_remove(GTK_CONTAINER(new_grid),box_yes_no);
    if (yes == 0)
    {
        gtk_container_remove(GTK_CONTAINER(new_grid),show_text);
        yes = 1;
    }

    gtk_widget_show_all(window);
    widget_show = 0;
    clear = 0;
    gtk_widget_set_sensitive(clear_button, TRUE);



}



void clear_surface(void) {
    cairo_t *cr;
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_destroy(cr);
    GdkRGBA color;
    gdk_rgba_parse(&color, "red");
    apply_color_to_number(&color, current_pred);
    gtk_widget_show_all(window);
}

gboolean configure_event_cb(GtkWidget *widget) {
    if (surface)
        cairo_surface_destroy(surface);

    surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
            CAIRO_CONTENT_COLOR,
            gtk_widget_get_allocated_width(widget),
            gtk_widget_get_allocated_height(widget));

    clear_surface();
    return TRUE;
}

gboolean draw_cb(GtkWidget *widget, cairo_t *cr) {
    int dimX, dimY;
    gtk_widget_get_size_request(widget, &dimX, &dimY);

    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    return FALSE;
}

void draw_brush(GtkWidget *widget, gdouble x, gdouble y) {
    cairo_t *cr;
    cr = cairo_create(surface);

    // Set line properties
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 20.0); 
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND); 
    if (previous_x != -1 && previous_y != -1) {
        cairo_move_to(cr, previous_x, previous_y);
        cairo_line_to(cr, x, y);
        cairo_stroke(cr);
    }

    cairo_destroy(cr);
    gtk_widget_queue_draw(widget);
    previous_x = x;
    previous_y = y;

}


gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event) {

    if (surface == NULL)
        return FALSE;

    if (event->button == GDK_BUTTON_PRIMARY) {

        draw_brush(widget, event->x, event->y);
    }


    GdkRGBA color;
    gdk_rgba_parse(&color, "red");

    apply_color_to_number(&color, current_pred);


    return TRUE;

}
void resize2(char * path, int h, int w)
{
    SDL_Surface * src = IMG_Load(path);
    SDL_Surface * dst = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
    int e = SDL_BlitScaled(src,NULL,dst,NULL);
    if(e == -1)
        errx(EXIT_FAILURE,"resize failed");
    IMG_SavePNG(dst,path);
}
GtkWidget*  show_title2()
{
    // Création de la box pour le titre
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_style_context_add_class(gtk_widget_get_style_context(title_box), "titre");
    // Ajout du titre 'OCR' avec une ligne en dessous
    GtkWidget *ocr_label = gtk_label_new("OCR PROJET");
    gtk_widget_set_valign(ocr_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(title_box), ocr_label, TRUE, FALSE, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(ocr_label), "ocr2");
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(ocr_label, GTK_ALIGN_CENTER);
    return title_box;

}

GtkWidget*  show_title()
{
    // Création de la box pour le titre
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_style_context_add_class(gtk_widget_get_style_context(title_box), "titre");
    // Ajout du titre 'OCR' avec une ligne en dessous
    GtkWidget *ocr_label = gtk_label_new("OCR PROJET");
    gtk_widget_set_valign(ocr_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(title_box), ocr_label, TRUE, FALSE, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(ocr_label), "ocr");
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(ocr_label, GTK_ALIGN_CENTER);
    return title_box;

}
GtkWidget* show_text_bon_numero()
{
    GtkWidget* Box_Bon_num = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 60);
    GtkWidget *num = gtk_label_new("Bon numero ? ");
    gtk_widget_set_valign(num, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(Box_Bon_num), num, TRUE, FALSE, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(num), "ocr3");
    gtk_widget_set_halign(num, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(Box_Bon_num, GTK_ALIGN_CENTER);
    return Box_Bon_num;

}


void button_press_num(GtkWidget *widget, gpointer user_data) {


    int dimX, dimY;
    gtk_widget_get_size_request(widget, &dimX, &dimY);

    GtkWidget *entry = GTK_WIDGET(user_data);

    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    int number = atoi(text);


    g_print("\n%s\n",path_screenshot);


    double* liste = load_image2(1,"../make_solve/Images_mnist/screenshot0.bmp");

    double input [10];
    for (int i = 0; i<10; i++)
    {
        if (i == number)
        {
            input[i] = 1;
        }
        else
        {
            input[i] =  0;
        }

    }
    learn2(liste, input, 0.001);
    g_print("Nombre analysé : %d\n", number);
    clear_surface();
    close_menu();


}

GtkWidget* show_widget_label() {
    // Création du champ d'entrée

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Entrez la bonne reponse");

    // Création du bouton
    GtkWidget *button = gtk_button_new_with_label("Valide");
    g_signal_connect(button, "clicked", G_CALLBACK(button_press_num), entry);

    // Création de la boîte horizontale
    GtkWidget* box  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_hexpand(entry, FALSE);
    gtk_widget_set_hexpand(button, FALSE);
    gtk_widget_set_vexpand(entry, FALSE);
    gtk_widget_set_vexpand(button, FALSE);
    gtk_widget_set_valign(entry, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER); 
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);
    // Configuration des propriétés d'allocation d'espace
    gtk_widget_set_hexpand(box, FALSE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    return box;
}


void button_yes_func()
{
    widget_show = 1;
    yes = 1;

    /*int number = current_pred;
      g_print("%i",number);
      g_print("\n%s\n",path_screenshot);
      double* liste = load_image2(1,"../make_solve/Images_mnist/screenshot0.bmp");
      double input [10];
      for (int i = 0; i<10; i++)
      {
      if (i == number)
      {
      input[i] = 1;
      }
      else
      {
      input[i] =  0;
      }

      }

      h_weight = malloc(HIDDEN * sizeof(double*));
      for(size_t i = 0; i<HIDDEN; i++)
      h_weight[i] = calloc(INPUT,sizeof(double));

      o_weight = malloc(OUTPUT * sizeof(double*));
      for(size_t i = 0; i<OUTPUT; i++)
      o_weight[i] = calloc(HIDDEN,sizeof(double));

      h_bias = calloc(HIDDEN, sizeof(double));
      o_bias = calloc(OUTPUT, sizeof(double));
      insert_list2(HIDDEN, INPUT, "hweight.txt", h_weight);
      insert_list2(OUTPUT, HIDDEN, "oweight.txt", o_weight);
      insert_list2(1,OUTPUT, "obias.txt", &o_bias);
      insert_list2(1,HIDDEN, "hbias.txt", &h_bias);

      learn2(liste, input, 0.001);

      replace2(HIDDEN,INPUT,"hweight.txt",h_weight);
      replace2(OUTPUT,HIDDEN, "oweight.txt",o_weight);
      replace2(1,HIDDEN,"hbias.txt", &h_bias);
      replace2(1,OUTPUT,"obias.txt", &o_bias);

      for (size_t i = 0; i < HIDDEN; i++) {
      free(h_weight[i]);
      }
      free(h_weight);


      for (size_t i = 0; i < OUTPUT; i++) {
      free(o_weight[i]);
      }
      free(o_weight);


      free(h_bias);
      free(o_bias);*/
    clear_surface();
    close_menu();


}


GtkWidget* button_yes;
void button_no_func(GtkWidget* button)
{
    yes = 0;

    widget_show = 1;
    gtk_widget_set_sensitive(button, FALSE);
    gtk_widget_set_sensitive(button_yes, FALSE);

    show_text= show_widget_label();



    label_text  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 70);
    gtk_widget_set_hexpand(label_text, TRUE);

    gtk_grid_attach(GTK_GRID(new_grid), show_text, 0,6,1,1);
    gtk_grid_attach(GTK_GRID(new_grid), label_text, 0,7,1,1);
    gtk_widget_set_halign (box,GTK_ALIGN_CENTER);
    gtk_widget_set_halign (chiffre_box,GTK_ALIGN_CENTER);
    gtk_widget_set_halign (frame,GTK_ALIGN_CENTER);
    gtk_widget_set_halign (titre,GTK_ALIGN_CENTER);
    gtk_widget_set_halign (box_yes_no,GTK_ALIGN_CENTER);
    gtk_widget_set_halign (show_text_bon_numeros,GTK_ALIGN_CENTER);

    gtk_widget_show_all(window);



}

GtkWidget* show_yes_or_not()
{
    clear = 1;
    gtk_widget_set_sensitive(clear_button, FALSE);
    GtkWidget* Box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,50);
    button_yes = gtk_button_new_with_label("OUI");
    g_signal_connect(button_yes, "clicked", G_CALLBACK(button_yes_func), NULL);
    gtk_box_pack_start(GTK_BOX(Box), button_yes, FALSE, FALSE, 0);

    gtk_style_context_add_class(gtk_widget_get_style_context(button_yes), "buttonYes");

    GtkWidget *button_no = gtk_button_new_with_label("NON");
    g_signal_connect(button_no, "clicked", G_CALLBACK(button_no_func), NULL);
    gtk_box_pack_start(GTK_BOX(Box), button_no, FALSE, FALSE, 0);

    gtk_style_context_add_class(gtk_widget_get_style_context(button_no), "buttonNo");


    gtk_widget_set_halign(button_no, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(button_yes, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(Box, GTK_ALIGN_CENTER);



    return Box;



}




gboolean button_release_event_cb(GtkWidget *widget, GdkEventButton *event) {
    if (clear != 0) return FALSE;
    if (surface == NULL)
        return FALSE;

    if (event->button == GDK_BUTTON_PRIMARY) {



        GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0,
                gdk_window_get_width(gtk_widget_get_window(widget)),
                gdk_window_get_height(gtk_widget_get_window(widget)));


        char str[100];
        sprintf(str, "../make_solve/Images_mnist/screenshot0.bmp");
        path_screenshot= g_strdup_printf("%s", str);




        // Sauvegarder l'image au format BMP
        gdk_pixbuf_save(pixbuf, path_screenshot, "bmp", NULL, NULL);

        resize2(path_screenshot, 28, 28);
        current_pred = prediction2(1,path_screenshot);





        GdkRGBA color;
        gdk_rgba_parse(&color, "green");

        apply_color_to_number(&color, current_pred);

        show_text_bon_numeros = show_text_bon_numero();
        gtk_grid_attach (GTK_GRID(new_grid), show_text_bon_numeros,0,4,1,1);

        box_yes_no = show_yes_or_not();
        gtk_container_set_border_width(GTK_CONTAINER(box_yes_no),20);
        gtk_grid_attach(GTK_GRID(new_grid), box_yes_no,0,5,1,1);






        g_free(path_screenshot);
        g_object_unref(pixbuf);

        gtk_widget_show_all(window);

        previous_x = -1;
        previous_y = -1;
    }

    return TRUE;
}
gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event) {
    if (surface == NULL)
        return FALSE;

    if (event->state & GDK_BUTTON1_MASK) {
        draw_brush(widget, event->x, event->y);
    }

    return TRUE;
}


void apply_css_on_frame(GtkWidget* widget)
{
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    // Ajout pour le débogage

    const char *css_data =
        "* { background-image:none; border: 4px solid #000000;}";
    gtk_css_provider_load_from_data(cssProvider, css_data, -1, NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(cssProvider);


}




void destroy_all_children(GtkContainer *container) {
    GList *children, *iter;

    children = gtk_container_get_children(container);
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }

    g_list_free(children);
}


void restart_application() {
    execlp("./graphics", "./graphics", NULL);
    exit(EXIT_FAILURE);
}

void quit_prog()
{

    gtk_widget_destroy(GTK_WIDGET(window));
    restart_application();
}

void activate() {


    // Ajout d'un espace sur le cote droit de la fenetre
    /*
       vide = gtk_box_new(GTK_ORIENTATION_VERTICAL, 70); 
       gtk_grid_attach(GTK_GRID(grid), vide, 3, 1, 1, 1);
       gtk_widget_set_vexpand(vide, TRUE);
       gtk_grid_set_column_spacing(GTK_GRID(grid), 100);
       gtk_container_set_border_width(GTK_CONTAINER(vide), 300);*/

    // Faut pas oublier de le detache de la grid quand on sort 

    new_grid = gtk_grid_new();
    gtk_container_remove(GTK_CONTAINER(window),grid);
    gtk_container_add(GTK_CONTAINER(window),new_grid);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 60);
    chiffre_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);

    clear_button = gtk_button_new_with_label("CLEAR");
    GtkWidget* quit = gtk_button_new_with_label("QUIT");

    GtkWidget* box_clear = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 60);
    gtk_container_add(GTK_CONTAINER(box_clear), clear_button);
    gtk_container_add(GTK_CONTAINER(box_clear), quit);


    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);




    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(drawing_area),170,170);

    gtk_container_add(GTK_CONTAINER(frame), drawing_area);


    gtk_widget_set_size_request(GTK_WIDGET(frame),200,200);
    GdkRGBA color;
    gdk_rgba_parse(&color, "red");

    for (int i = 1; i <= 9; i++) {
        char label_text[4];
        sprintf(label_text, "%d", i);

        GtkWidget *button = gtk_button_new_with_label(label_text);
        gtk_widget_set_sensitive(button, FALSE); // Rendre le bouton non cliquable
        gtk_box_pack_start(GTK_BOX(chiffre_box), button, FALSE, FALSE, 5);
        apply_button_color(button, &color);
        gtk_widget_set_halign (button,GTK_ALIGN_CENTER);

    }    

    gtk_container_set_border_width(GTK_CONTAINER(frame),100);


    gtk_box_pack_start(GTK_BOX(box), clear_button, FALSE, FALSE, 0); 
    gtk_box_pack_start(GTK_BOX(box),quit, FALSE,FALSE,0);
    gtk_widget_set_halign (box,GTK_ALIGN_CENTER);
    gtk_widget_set_halign (chiffre_box,GTK_ALIGN_CENTER);
    gtk_widget_set_halign (frame,GTK_ALIGN_CENTER);
    gtk_widget_set_halign (box_clear,GTK_ALIGN_CENTER);



    vide = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 70);
    gtk_widget_set_hexpand(vide, TRUE);




    //  show_text_bon_numeros =  show_text_bon_numero();

    titre = show_title2();




    gtk_container_set_border_width(GTK_CONTAINER(titre),20);
    gtk_container_set_border_width(GTK_CONTAINER(box_clear),70);


    //gtk_container_set_border_width(GTK_CONTAINER(box), 0);  
    //  gtk_container_set_border_width(GTK_CONTAINER(frame), 20);   
    gtk_grid_attach(GTK_GRID(new_grid),titre, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(new_grid), frame, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(new_grid), chiffre_box, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(new_grid), box_clear, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(new_grid), vide, 0, 6, 1, 1);


    g_signal_connect(quit,"clicked",G_CALLBACK(quit_prog),NULL);
    g_signal_connect(clear_button, "clicked", G_CALLBACK(clear_surface), NULL);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(drawing_area, "configure-event", G_CALLBACK(configure_event_cb), NULL);
    g_signal_connect(drawing_area, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(button_press_event_cb), NULL);
    g_signal_connect(drawing_area, "button-release-event", G_CALLBACK(button_release_event_cb), NULL);

    gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area)
            | GDK_BUTTON_PRESS_MASK
            | GDK_BUTTON_RELEASE_MASK
            | GDK_POINTER_MOTION_MASK);



    gtk_widget_show_all(window);

    apply_back_ground_color("image_test/Blanc.png");

    GdkRGBA red = {
        1.0,
        0.0,
        0.0,
        1.0
    }; 
    apply_button_color(quit, &red);
    apply_css_on_frame(frame);
}

//------------------------------------- ROTATE GRAPHICS WIDGET --------------------------------------



GtkWidget* box_rotate;

void remove_widget()
{
    gtk_container_remove(GTK_CONTAINER(grid),box_rotate);
}

void button_press(gpointer user_data) {
    GtkWidget *entry = GTK_WIDGET(user_data);
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    int num = atoi(text);
    g_print("Nombre analysé : %d\n", num);
    surface_to_rotate_shear(&img,num);
    enqueue_img();
}

void auto_press() {
    if(rotated)
    {
        if(angle)
        {
            surface_to_rotate_shear(&img,angle);
            enqueue_img();
        }
    }
    else
        auto_turn();
}



void show_widget() {
    // Création du champ d'entrée
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Entrez un nombre");

    // Création du bouton
    GtkWidget *button = gtk_button_new_with_label("Turn");
    GtkWidget *button_auto = gtk_button_new_with_label("Automatic turn");
    g_signal_connect(button, "clicked", G_CALLBACK(button_press), entry);
    g_signal_connect(button_auto, "clicked", G_CALLBACK(auto_press), entry);

    // Création de la boîte horizontale
    box_rotate = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gtk_widget_set_hexpand(entry, FALSE);
    gtk_widget_set_hexpand(button, FALSE);
    gtk_widget_set_hexpand(button_auto, FALSE);
    gtk_widget_set_vexpand(entry, FALSE);
    gtk_widget_set_vexpand(button, FALSE);
    gtk_widget_set_vexpand(button_auto, FALSE);
    gtk_widget_set_valign(entry, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER); 
    gtk_widget_set_valign(button_auto, GTK_ALIGN_CENTER); 
    gtk_box_pack_start(GTK_BOX(box_rotate), entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_rotate), button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_rotate), button_auto, FALSE, FALSE, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(button_auto),"Turn");

    // Configuration des propriétés d'allocation d'espace
    gtk_widget_set_vexpand(box_rotate, FALSE);
    gtk_widget_set_valign(box_rotate, GTK_ALIGN_CENTER);

    // Attache de la boîte à la grille
    gtk_grid_attach(GTK_GRID(grid), box_rotate, 3, 1, 1, 1);
}





/*

// -------------------------------------- CLEAR DIRECTORY --------------------------------------

void clearDirectory(char *directoryPath) {
DIR *dir;
struct dirent *entry;
dir = opendir(directoryPath);
if (dir == NULL) {
return;
}

while ((entry = readdir(dir)) != NULL) {
if (entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN) {

char filePath[PATH_MAX];
snprintf(filePath, sizeof(filePath), "%s/%s", directoryPath, entry->d_name);
if (remove(filePath) != 0) {
perror("Error deleting file");
}
}
}
closedir(dir);
}


*/

// -------------------------------------- BUTTON MANAGER --------------------------------------

// Fonction appelée lorsque l'un des boutons est cliqué

int show = 0;

void button_clicked(GtkWidget* widget, gpointer data) {


    int dimX, dimY;
    gtk_widget_get_size_request(widget, &dimX, &dimY);


    int button_id = GPOINTER_TO_INT(data);
    if (show == 1)
    {
        show = 0;
        remove_widget();
    }

    int kernel = 21;
    switch (button_id) { // Temporaire 
        case 1:  //FILTRE
            speed_ada(img,kernel);
	    //sauvola(img);
            break;
        case 2: // HOUGH
            surface_to_hough(img,&angle);
            break;
        case 3: // ROTATION
            show_widget();
            show = 1;
            break;
        case 4 : //GRID DISPLAY
            speed_ada(img,kernel);
            print_grid(img);
            break;
        case 5: // CROP 
            speed_ada(img,kernel);
            extract_grid(&img);
            enqueue_grid();
            button_id = 10;
            break;
        case 6://FULL SOLUTION 
            if(solved == 0)
            {
                speed_ada(img,kernel);
                extract_grid(&img);
                send_solution("grid.png");
                solved++;
            }
            img = load_image("result.png");
            break;
        case 7://STEP BY STEP
            if(solved == 0)
            {
                speed_ada(img,kernel);
                extract_grid(&img);
                send_solution("grid.png");
                solved++;
            }
            new_step();
            break;
        case 8: // Load image
            file_selected();
            break;
        case 9: // BACKING
            dequeue(Q);
            break;
        case 10: 
            activate();
            break;
        case 11:
            gtk_main_quit();
            break;
        default:
            break;
    }
    if(button_id <= 6)
        //SAVING PROCESS
        enqueue_img();
}







// ------------------------------------------ EVENT --------------------------------------------





void on_window_destroy() {
    delete_files_with_path("testImageJules/image.jpg");

    gtk_main_quit();
}




// ------------------------------------------ MAIN --------------------------------------------
int main(int argc, char *argv[]) {

    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Load CSS
    load_css();

    // Création de la fenêtre principale
    window  =  gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GdkWindow *gdk_window = gtk_widget_get_window(GTK_WIDGET(window));
    //gdk_window_get_geometry(gdk_window, NULL, NULL, &window_width, &window_height); //recup les dim de la window
    window_height = window_width = 1000;
    gtk_window_resize(GTK_WINDOW(window), window_width, window_height);
    gtk_window_set_title(GTK_WINDOW(window), "OCR PROJET");
    //gtk_window_fullscreen(GTK_WINDOW(window)); // Fullscreen (tu peux le deactiver pr faire des tests)


    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);


    // Création de la grille
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 50);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 50);

    // Création d'une box ou on va stock des buttons
    button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
    //     //gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_box, 0, 1, 1, 1);
    //
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 50);

    // Ajout de boutons à la box
    for (int i = 1; i <= 11; i++) {
        char button_label[15];
        if (i == 1)
        {
            sprintf(button_label, "BLACK & WHITE");
        }
        if (i == 2)
        {
            sprintf(button_label, "HOUGH");
        }
        if (i == 3)
        {
            sprintf(button_label, "ROTATION");
        }
        if (i == 4)
        {
            sprintf(button_label, "GRID DISPLAY");
        }
        if(i == 5)
        {
            sprintf(button_label, "CROP");
        }
        if (i == 6)
        {
            sprintf(button_label, "FULL SOLUTION");
        }
        if(i == 7)
        {
            sprintf(button_label, "STEP BY STEP");
        }
        if (i == 8)
        {
            sprintf(button_label, "LOAD IMAGE");
        }   
        if (i == 9)
        {
            sprintf(button_label, "BACK");
        }
        if (i == 10)
        {
            sprintf(button_label, "DRAW");
        }
        if (i == 11)
        {
            sprintf(button_label, "QUIT");
        }
        GtkWidget *button = gtk_button_new_with_label(button_label);
        g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), GINT_TO_POINTER(i));
        gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);
        gtk_widget_set_size_request(button, -1, 30);     

        //gtk_grid_attach(GTK_GRID(grid), button, 0, i-1, 1, 1);

    }  

    gtk_style_context_add_class (gtk_widget_get_style_context(button_box),"buttons");

    // Création de la ligne de séparation
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_grid_attach(GTK_GRID(grid), separator, 1, 1, 1, 1);
    gtk_style_context_add_class(gtk_widget_get_style_context(separator), "separator");
    gtk_widget_set_halign (button_box,GTK_ALIGN_CENTER);

    // Zone pour l image de la grille de sudoku
    path_image = "image.jpg";
    GtkWidget *image = load_image_from_path_and_adjust_resolution(path_image);  // chemin argv[1]
    gtk_grid_attach(GTK_GRID(grid), image, 2, 1, 1, 1);
    apply_back_ground_color(path_image);
    img = load_image(starter);
    save_image(image);
    gtk_widget_set_halign (image ,GTK_ALIGN_CENTER);


    // Création de la box pour le titre
    gtk_grid_attach(GTK_GRID(grid), show_title(), 2, 0, 3, 1);
    // Réglez l'expansion de la zone de dessin



    //gtk_container_set_border_width(GTK_CONTAINER(title_box), 60); 

    // Réglez l'expansion de la zone de dessin
    gtk_widget_set_hexpand(image, TRUE);
    gtk_widget_set_vexpand(image, TRUE);

    // Affichage de tous les éléments
    gtk_widget_show_all(window);

    // Lancement de la boucle principale de GTK

    gtk_main();

    return 0;
}
