#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT 2
#define HIDDEN 3
#define OUTPUT 1


//definition of weight and bias array
double h_weight[HIDDEN][INPUT];
double h_bias[HIDDEN];
double o_weight[OUTPUT][HIDDEN];
double o_bias[OUTPUT];
double inputs1[4][2] = { {0,0} , {0,1}, {1,0}, {1,1} };
double outputs1[4][1] = {{0},{1},{1},{0}};



//Choose a random number between -1 and 1
double randomchoice() 
{
    int randomInt = rand();
    double randomDouble = ((double)randomInt / RAND_MAX) - 1;
    return randomDouble;
}



//sigmoid fonction
double sigmoid(double x) 
{
    return 1.0 / (1.0 + exp(-x));
}

//derivate sigmoid function
double sigmoid_prime(double x)
{
    return x * (1-x);
}

//return the prediction of the neural network
double predict(double inputs[INPUT])
{
    double hiddens[HIDDEN];
    for(size_t i =0; i<HIDDEN;i++)
    {
        double hidden = 0;
        for(size_t j = 0; j<INPUT;j++)
            hidden+= h_weight[i][j] * inputs[j];
        hidden = sigmoid(hidden +h_bias[i]);
        hiddens[i] = hidden;
    }

    double output = 0;
    output =0;
    for(size_t j =0; j < HIDDEN; j++)
        output+= o_weight[0][j] * hiddens[j];
    output = sigmoid(output + o_bias[0]);
    return output;
}

//The function teaches the neural network by adjusting biases and weights
//using sigmoid fonctions
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

//Main fonction: print the results
int main()
{
    srand(time(NULL));
    size_t i =0;
    printf("💻 Start of neural network training...\n\n"); 
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
            o_weight[z][j] = randomchoice();
        }

        o_bias[z] = 0;
        z++;
    }


    for(size_t i =0; i < 1000000; i++)
    {
        for(size_t j =0; j <4; j++)
        {  
            learn(inputs1[j], outputs1[j], 0.001);
        }
        if((i+1)%200000 ==0)
        {
            double cost =0;
            for(size_t j=0; j<4;j++)
            {
                double o = predict(inputs1[j]);
                cost += pow((outputs1[j][0] -o), 2);
            }
            cost/= 4;
            printf("🏋️  %lu/1000000 mean squared error: %f\n", i+1, cost);
        }
    }
    printf("\n");
    printf("🔎 Checking results...\n\n");
    for(size_t i=0; i<4;i++)
    {
        double result = predict(inputs1[i]);
        printf("✅ for input %lu expected %f predicted %f\n", i, 
			outputs1[i][0],result);
    }
}
