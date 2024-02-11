#include "ocrmnist.h"

int main(int argc, char** argv)
{
    if (argc == 1)
        errx(1, "Rajouter un argument");
    chargement();
    double* inputs = load_image_revert(1,argv);
    double* result = predict(inputs);
    int max = 0;
    for(int i = 1; i<10;i++)
    {
        if(result[max] < result[i])
            max = i;
    }
    printf("🔮 Prédiction: %i\n", max);

    return EXIT_SUCCESS;

}
