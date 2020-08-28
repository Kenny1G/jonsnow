#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct triangle
{
	int a;
	int b;
	int c;
};
typedef struct triangle triangle;

typedef struct{
   triangle tr;
   double area;
} triangleArea;

double area(triangle tr);
triangleArea* quickSort(triangleArea* array, int n);

void sort_by_area(triangle* tr, int n) {
	/**
	* Sort an array a of the length n
	*/
   triangleArea* toBeSorted = malloc(sizeof(triangleArea) * n);
   for (int i = 0; i < n; ++i) {
      toBeSorted[i].tr = tr[i];
      toBeSorted[i].area = area(tr[i]);
   }

   triangleArea* sorted; 
   sorted = quickSort(toBeSorted, n);
   for (int i = 0; i < n; i++) {
      printf("%lf \n", sorted[i].area);
      tr[i]  = sorted[i].tr;
	}
   free(sorted);
}

double area(triangle tr) {
   double p = tr.a + tr.b + tr.c;
   p = p/2;
   return sqrt(p * (p - tr.a) * (p - tr.b) *  (p - tr.c));
}

triangleArea* quickSort(triangleArea* array, int n) {
   if (n == 1) {
      return array;
   }
   int midNum = n/2;

   triangleArea mid = array[midNum];
   triangleArea* smaller = malloc(sizeof(triangleArea) * (n + 1)); 
   triangleArea* bigger = malloc(sizeof(triangleArea) * n);

   int s = 0;
   int b = 0;


   for (int i = 0; i < n; ++i) {
      triangleArea curr = array[i];
      if (curr.area <= mid.area && i != midNum) {
         smaller[s++] = curr;
      } else if(curr.area > mid.area){
         bigger[b++] = curr;
      }
   }
   free(array);

   
   if (s != 0) smaller = quickSort(smaller, s);
   if(b!= 0) bigger = quickSort(bigger ,b);

   triangleArea* finale = malloc(sizeof(triangleArea) * n);
   memcpy(finale, smaller, sizeof(triangleArea) * s);
   finale[s] = mid;
   if (b != 0) memcpy(finale + (s+1), bigger, sizeof(triangleArea) * b); 
   if (smaller) free(smaller);
   if (bigger && b!= 0) free(bigger);

   return finale;
}
int main()
{
	int n;
	scanf("%d", &n);
	triangle *tr = malloc(n * sizeof(triangle));
	for (int i = 0; i < n; i++) {
		scanf("%d%d%d", &tr[i].a, &tr[i].b, &tr[i].c);
	}
	sort_by_area(tr, n);
   printf("\n\n\n\n\n\n");
	for (int i = 0; i < n; i++) {
		printf("%d %d %d\n", tr[i].a, tr[i].b, tr[i].c);
	}
   free(tr);
	return 0;
}
