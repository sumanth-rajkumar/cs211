#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double **mallocMatrix(int row, int column) 
{
    double **result = (double**) malloc(sizeof(double *) *row);
    for(int i = 0; i < row; i++)
    {
        result[i] = (double *) malloc(sizeof(double) *column);

    }
    return result;
}
void freeMatrix(double **matrix, int row) 
{
    
    for(int i = 0; i < row; i++)
    {
        free(matrix[i]); 

    }
    free(matrix);
}
double **transpose(double **m, int row, int column) 
{
    double **transpose = mallocMatrix(column, row);
    for(int i = 0; i < row; i++) 
    {
        for(int j = 0; j < column; j++) 
        {
            transpose[j][i] = m[i][j];
            
        }
    }
    return transpose;
}
double **multiply(double **m1, double **m2, int row1, int column1, int row2 , int column2) 
{
    double **r = mallocMatrix(row1, column2);

    for(int i = 0; i < row1; i++) 
    {
        for(int j = 0; j < column2; j++) 
        {
            r[i][j] = 0;
            for(int k = 0; k < row2; k++) 
            {
                r[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return r;
}
double *vecMatMultiply(double **m, double *v, int row, int column) 
{
    double * t = (double *) malloc(sizeof(double) *row);

    
    for(int i = 0; i < row; i++) 
    {
        t[i] = 0;
        for(int j = 0; j < column; j++) 
        {
            t[i] += m[i][j] * v[j];
        }
    }
    return t;
}
double **gjInverse(double **m, int size) 
{

    double **iM = (double **) malloc(sizeof(double *) *size);

    for(int i = 0; i < size; i++) 
    {
        iM[i] = (double *) malloc(sizeof(double) *size);
        for(int j = 0; j < size; j++) 
        {
            if(i == j) 
            {
                iM[i][j] = 1;
            } 
            else 
            {
                iM[i][j] = 0;
            }
        }
    }
    for(int i = 0; i < size; i++) 
    {
        if(m[i][i] != 1) 
        {
            double r = 1/m[i][i];
        
            for(int j = 0; j < size; j++) 
            {
                m[i][j] *= r;
                iM[i][j] *= r; 

                if(m[i][j] == -0)
                {
                    m[i][j] = 0;
                }
                if(iM[i][j] == -0) 
                {
                    iM[i][j] = 0;
                }
            }
        }
        for(int k = i+1; k < size; k++) 
        {
        
            if(m[k][i] != 0) 
            {
                double fa = m[k][i] * -1;
               
                for(int s = 0; s < size; s++) 
                {
                    m[k][s] += (fa*m[i][s]);
                    iM[k][s] += (fa*iM[i][s]);

                    if(m[k][s] == -0)
                    {
                        m[k][s] = 0;
                    }
                    if(iM[k][s] == -0) 
                    {
                        iM[k][s] = 0;
                    }

                }
            }
        }
    }
    for(int i = size-1; i >= 0; i--) 
    {
     
       if(m[i][i] != 1) 
       {
           double r = 1/m[i][i];

           for(int j = 0; j < size; j++) 
           {
               m[i][i] *= r;
               iM[i][i] *= r;

               if(m[i][j] == -0)
               {
                    m[i][j] = 0;
               }
               if(iM[i][j] == -0) 
               {
                   iM[i][j] = 0;
               }
           }
        }
        for(int k = i-1; k >= 0; k--) 
        {
            
            if(m[k][i] != 0) 
            {
                double fa = m[k][i] * -1;
               
                for(int s = 0; s < size; s++) 
                {
                    m[k][s] += (fa*m[i][s]);
                    iM[k][s] += (fa*iM[i][s]);

                    if(m[k][s] == -0)
                    {
                        m[k][s] = 0;
                    }
                    if(iM[k][s] == -0) 
                    {
                        iM[k][s] = 0;
                    }

                }
            }
        }
    }
    return iM;
}

int main(int argc, char **argv)  
{
    FILE *f1;
    FILE *f2;
    
    char l[20] = {}; 

    if(argc != 3) 
    {
        printf("error\n");
        exit(0);
    }

    f1 = fopen(argv[1],"r");
    f2 = fopen(argv[2],"r");

    if(f1 == NULL || f2 == NULL) 
    {
        printf("error\n");
        exit(0);
    }

    fscanf(f1,"%s\n",l);

  
    if((strncmp(l,"train", 5) != 0)) 
    {
        printf("error\n");
        exit(0);
    }

    int column;
    int row;
    
    fscanf(f1,"%d\n",&column);
    fscanf(f1,"%d\n",&row);
    column++; 

    
    double **train = mallocMatrix(row, column);
    double **real = mallocMatrix(row, column);
    double *prices = (double *) malloc(sizeof(double *) *row);


    for(int i = 0; i < row; i++) 
    {
        train[i][0] = 1; 
        for(int j = 0; j < column; j++) 
        {
            double v;
            fscanf(f1,"%lf ",&v);
            real[i][j] = v;
        }
        fscanf(f1,"\n");
    }

   
    for(int i = 0; i < row; i++) 
    {
        prices[i] = real[i][column-1];
        for(int j = 1; j < column; j++) 
        {
            train[i][j] = real[i][j-1];
        }
    }


    double **tr = transpose(train, row, column);
    double **m1 = multiply(tr, train, column, row, row, column);
    double **in = gjInverse(m1, column);
    double **m2 = multiply(in, tr, column, column, column, row);
    double *v1 = vecMatMultiply(m2, prices, column, row);

    
    fscanf(f2,"%s\n",l);


    if(strncmp(l,"data", 4) != 0) 
    {
        printf("error\n");
        exit(0);
    }

    int row2;
    int col;
    fscanf(f2,"%d\n",&col);
    fscanf(f2,"%d\n",&row2);
    col++; 

    if(column != col)
    {
        printf("error\n");
        exit(0);
    }

    double **data = mallocMatrix(row2, col);

   
    for(int i = 0; i < row2; i++) 
    {
        data[i][0] = 1;
        for(int j = 1; j < col; j++) 
        {
            double s;
            fscanf(f2,"%lf ",&s);
            data[i][j] = s;
        }
        fscanf(f2,"\n");
    }
    
    double *v2 = vecMatMultiply(data,v1,row2,col);

    for(int i = 0; i < row2; i++) 
    {
        printf("%.0f\n",v2[i]);
    }

    freeMatrix(train, row);
    freeMatrix(real, row);
    free(prices);
    freeMatrix(tr, column);
    freeMatrix(m1, column);
    freeMatrix(in, column);
    freeMatrix(m2, column);
    free(v1);
    free(v2);
    freeMatrix(data, row2);

    return 0;
}
