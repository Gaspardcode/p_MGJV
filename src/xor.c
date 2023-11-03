#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT 4
#define HIDDEN 3
#define OUTPUT 1


//definition of weight and bias array
double h_weight[HIDDEN][INPUT];
double h_bias[HIDDEN];
double o_weight[OUTPUT][HIDDEN];
double o_bias[OUTPUT];
double inputs1[4][2] = { {0,0} , {0,1}, {1,0}, {1,1} };
double outputs1[4][1] = {{0},{1},{1},{0}};




double randomchoice() 
{
    int randomInt = rand();
    double randomDouble = (double)randomInt / RAND_MAX - 1;
    printf("%f\n",randomDouble);
    return randomDouble;
    /*int randomInt = rand(); // Génère un nombre aléatoire entre 0 et RAND_MAX
    double randomDouble = (double)randomInt / RAND_MAX; // Normalise entre 0 et 1
    randomDouble = 2 * randomDouble - 1; // Normalise entre -1 et 1
    
    printf("%f\n",randomDouble);
    return randomDouble;*/
}




double sigmoid(double x) 
{
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_prime(double x)
{
    return x * (1-x);
    //return sigmoid(x)*(1-sigmoid(x));
}

double predict(double inputs[HIDDEN])
{
    double hiddens[HIDDEN];
    for(size_t i =0; i<HIDDEN;i++)
    {
        double hidden = 0;
        for(size_t j = 0; j<INPUT;j++)
            hidden+= h_weight[i][j] * inputs[j];
        hiddens[i] = hidden;
    }

    double outputs[OUTPUT];
    double output = 0;
    for(size_t i =0; i<OUTPUT;i++)
    {
        output =0;
        for(size_t j =0; j < HIDDEN; j++)
            output+= o_weight[i][j] * hiddens[j];
        output = sigmoid(output + o_bias[i]);
        outputs[i] = output;
    }
    return outputs[0];
}

void learn(double inputs[INPUT], double targets[OUTPUT], double alpha)
{
    double hiddens[HIDDEN];
    for(size_t i=0; i< HIDDEN;i++)
    {
        double hidden = 0;
        for(size_t j =0;j<INPUT; j++)
        {
            hidden+= h_weight[i][j] * inputs[j];
        }
        hidden = sigmoid(hidden + h_bias[i]);
        hiddens[i] = hidden;
    }

    double outputs[OUTPUT];
    for(size_t i =0; i< OUTPUT; i++)
    {
        double output = 0;
        for(size_t j = 0; j<HIDDEN;j++)
            output+= o_weight[i][j] *hiddens[j];
        output = sigmoid(output + o_bias[i]);
        outputs[i] = output;
    }

    double errors[OUTPUT];
    for(size_t i =0; i<OUTPUT;i++)
    {
        double error = targets[i] - outputs[i];
        errors[i] = error;
    }

    double derrors[OUTPUT];
    for(size_t i =0;i<OUTPUT;i++)
    {
        double derror = errors[i] * sigmoid_prime(outputs[i]);
        derrors[i] = derror;
    }

    double ds[HIDDEN];
    for(size_t i = 0;i <HIDDEN;i++)
        ds[i] = 0;
    for(size_t i =0; i<OUTPUT;i++)
    {
        for(size_t j =0; j <HIDDEN; j++)
            ds[j] += derrors[i] * o_weight[i][j] * sigmoid_prime(hiddens[j]);
    }

    for(size_t i=0; i<OUTPUT;i++)
    {
        for(size_t j=0;j<HIDDEN;j++)
            o_weight[i][j] += alpha * hiddens[j] * derrors[i];
        o_bias[i] += alpha * derrors[i];
    }

    for(size_t i=0; i<HIDDEN; i++)
    {
        for(size_t j =0;j <INPUT;j++)
            h_weight[i][j] += alpha * inputs[j] * ds[i];
        h_bias[i]+= alpha *ds[i];
    }

}


int main()
{
    srand(time(NULL));
    size_t i =0;

    while(i < HIDDEN)
    {
    
        for (size_t j =0; j < INPUT; j++)
        {
            h_weight[i][j] = randomchoice();
        }
        h_bias[i] = 0;
        i++;
    }  

    size_t z = 0;
    while(z < OUTPUT)
    {
        for (size_t j =0; j < HIDDEN; j++)
        {
            h_weight[z][j] = randomchoice();
        }

        o_bias[z] = 0;
        z++;
    }


    for(size_t i =0; i < 10000; i++)
    {
        size_t indexe[] = {0,1,2,3};
        for(size_t j =0; j <4; j++)
        {
            size_t random = indexe[rand()%4];   
            learn(inputs1[random], outputs1[random], 0.2);
        }
        if((i+1)%1000 ==0)
        {
            double cost =0;
            for(size_t j=0; j<4;j++)
            {
                double o = predict(inputs1[j]);
                cost += pow((outputs1[j][0] -o), 2);
            }
            cost/= 4;
            printf("%lu mean squared error: %f\n", i+1, cost);
        }
        if((i+1)%5000 ==0)
        {
            for(size_t i =0; i< HIDDEN;i++)
            {
                for(size_t j=0;j<INPUT;j++)
                    printf("%f\n", o_weight[i][j]);
            }
        }
    }
    printf("\n");
    for(size_t i=0; i<4;i++)
    {
        double result = predict(inputs1[i]);
        printf("for input %lu expected %f predicted %f\n", i, outputs1[i][0],result);
    }
}
