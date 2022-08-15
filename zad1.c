#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
//#define LAPTOPS_COUNT 4

//  gcc zad1.c -o zad1 -D LAPTOPS_COUNT=4
//  ./zad1 

typedef struct Laptop {
    unsigned long id;
    char Laptop_brand[100];
    double weight;
    double price;
} Laptop;


int random_generate(int min, int max){
   return min + rand()% (max - min + 1);
}
void random_Laptop(Laptop *p) {
    static int id_counter = 0;
    p->id = id_counter++;

    int length = random_generate(8, 12);
    p->Laptop_brand[0] = random_generate('A', 'Z');
    
  
    for (int j = 1; j <= (length-1)*2; j++)
    {
        if (j%2 == 0)
        {
            p->Laptop_brand[j] = random_generate('a', 'z');
        }else
        {
            p->Laptop_brand[j] = ' ';
        }
    }

    p->weight = random_generate(25, 10000)/1000.;
    p->price = random_generate(1000, 500000)/100.;
    

}

void print_Laptop(Laptop *p) {
    printf("Id:%lu, Weight: %.3lf, Price:%.2lf, Brand: %s\n",
      p->id, p->weight, p->price, p->Laptop_brand);
}

double totalLaptopWeight(Laptop* p){
    if (p == NULL)
    {
        fprintf(stderr, "Error\n");
        return -1;
    }
    
    double total_weight = 0.;
    for(int i = 0; i < LAPTOPS_COUNT; i++) {
        total_weight += p[i].weight;
    }
  
    return total_weight;
}

Laptop* minPriceItem(Laptop* p){
    if (p == NULL)
    {
        fprintf(stderr, "Error\n");
        return NULL;
    }
    

    double min  = p[0].price;
    int min_index = 0;
    for (size_t i = 1; i < LAPTOPS_COUNT; i++)
    {
        if (min > p[i].price)
        {
            min = p[i].price ;
            min_index = i;
        }
       
    }

    return &p[min_index];
}

int compare_Laptops(const void *p1, const void *p2) {
    const Laptop *prod1 = p1, *prod2 = p2;

    if(fabs(prod1->price - prod2->price) > 0.01) {
        return 0;
    } 
    return -1;
}

int main() { 
    Laptop *Laptop_list;
    Laptop_list = malloc(sizeof(Laptop)*LAPTOPS_COUNT);
    if(!Laptop_list) {
        perror("Malloc failed\n");
        return -1;
    }

    srand(time(NULL));

    for(int i = 0; i < LAPTOPS_COUNT; i++) {
        random_Laptop(Laptop_list + i);
    }

 
    for(int i = 0; i < LAPTOPS_COUNT; i++) {
        print_Laptop(Laptop_list + i);
    }

    printf("\nTotal weight of all laptops: %lf\n", totalLaptopWeight(Laptop_list));
    printf("Cheapest laptop:\n");
    print_Laptop(minPriceItem(Laptop_list));

    free(Laptop_list);
    
}
