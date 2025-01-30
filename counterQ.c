#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items {
    char item[50];
    float price;
    int quantity;
};

struct orders {
    char client[50];
    char date[50];
    int numOfItems;
    struct items itm[50];
};

void generateBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t   CounterQ. @ThisIzNeo");
    printf("\n\t ---------------------");
    printf("\nDate: %s", date);
    printf("\nOrdered To: %s", name);
    printf("\n");
    printf("------------------------------\n");
    printf("Items\t\tQty\t\tTotal\n");
    printf("------------------------------\n\n");
}

void generateBillBody(char item[30], int quantity, float price) {
    printf("%s\t\t%d\t\t%.2f\n", item, quantity, quantity * price);
}

void generateBillFooter(float total) {
    float discount = 0.15 * total;
    float ttc = total - discount;
    float extra = 0.09 * ttc, finalCost = ttc + 2 * extra;
    
    printf("\n------------------------------\n");
    printf("Value without Discount\t\t%.2f", total);
    printf("\nDiscount @15%%\t\t%.2f", discount);
    printf("\n\t\t\t-------");
    printf("\nNet Total\t\t%.2f", ttc);
    printf("\nCoast @9%%\t\t%.2f", extra);
    printf("\nTax @9%%\t\t%.2f", extra);
    printf("\n------------------------------\n");
    printf("Final Cost\t\t%.2f\n", finalCost);
}

int main() {
    int option, n;
    struct orders ord, order;
    char saveBill = 'y', contFlag = 'y', name[50];
    FILE *fp;
    
    while (contFlag == 'y') {
        #ifdef _WIN32
            system("cls"); // Windows
        #else
            system("clear"); // Linux/macOS
        #endif
        
        float total = 0;
        int invoiceFound = 0;
        
        printf("\t=========== CounterQ. @ThisIzNeo ===========\n");
        printf("\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoice");
        printf("\n4. Exit");
        printf("\n\nYour choice: ");
        scanf("%d", &option);
        fgetc(stdin);
        
        switch (option) {
            case 1:
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                printf("\nEnter the name of the client: ");
                fgets(ord.client, 50, stdin);
                ord.client[strlen(ord.client) - 1] = 0;
                strcpy(ord.date, __DATE__);
                printf("\nEnter the number of products: ");
                scanf("%d", &n);
                ord.numOfItems = n;
                
                for (int i = 0; i < n; i++) {
                    fgetc(stdin);
                    printf("\n\nEnter Product %d: ", i + 1);
                    fgets(ord.itm[i].item, 50, stdin);
                    ord.itm[i].item[strlen(ord.itm[i].item) - 1] = 0;
                    printf("Please enter the quantity: ");
                    scanf("%d", &ord.itm[i].quantity);
                    printf("Enter the unit price: ");
                    scanf("%f", &ord.itm[i].price);
                    total += ord.itm[i].quantity * ord.itm[i].price;
                }

                generateBillHeader(ord.client, ord.date);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].quantity, ord.itm[i].price);
                }
                generateBillFooter(total);

                printf("\nDo you want to save the invoice [y/n]: ");
                scanf(" %c", &saveBill);
                
                if (saveBill == 'y') {
                    fp = fopen("RestaurantBill.dat", "a+");
                    if (fwrite(&ord, sizeof(struct orders), 1, fp) == 1)
                        printf("\nSuccessfully saved");
                    else 
                        printf("\nError saving");
                    fclose(fp);
                }
                break;
                
            case 2:
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                fp = fopen("RestaurantBill.dat", "r");
                printf("\n  ***** Your Previous Invoices *****\n");
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    generateBillHeader(order.client, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].quantity, order.itm[i].price);
                        tot += order.itm[i].quantity * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                }
                fclose(fp);
                break;
                
            case 3:
                printf("Enter the name of the client: ");
                fgets(name, 50, stdin);
                name[strlen(name) - 1] = 0;
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                fp = fopen("RestaurantBill.dat", "r");
                printf("\t***** Invoice of %s *****\n", name);
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    if (!strcmp(order.client, name)) {
                        generateBillHeader(order.client, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].quantity, order.itm[i].price);
                            tot += order.itm[i].quantity * order.itm[i].price;
                        }
                        generateBillFooter(tot);
                        invoiceFound = 1;
                    }
                }
                if (!invoiceFound) {
                    printf("Sorry, the invoice for %s does not exist\n", name);
                }
                fclose(fp);
                break;
                
            case 4:
                printf("\n\t\t Follow @ThisIzNeo <3\n\n");
                exit(0);
                
            default:
                printf("Invalid option!");
        }
        printf("\nDo you want another operation? [y/n]: ");
        scanf(" %c", &contFlag);
    }
    printf("\n\t\t Follow @ThisIzNeo <3\n\n");
    return 0;
}
