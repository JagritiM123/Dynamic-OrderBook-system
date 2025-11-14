#include<stdio.h>   //for printf and scanf
#include "structure.h"  //custom header with struct definitions + function prototypes
#include<string.h>  //for string compare (strcmp) and string copy (strcpy)
#include<stdlib.h>  //for malloc and free
#include<time.h>    //for timestamps in order

int main()
{
    order_list stocks_list;     //declares a variable named stocks_list of type order_list
    initializing_OrderBook(&stocks_list);
    /*
    initializing the order book:
    sets buycount = 0
    sets sellcount = 0
    sets all pointers -> links and BST to NULL
    */
    char c;     
    while(1){   //this is an infinite so that the loop keeps running until q is the option selected
        printf("\nWelcome!!\n");
        printf("\nStart Bidding and Buying Stocks\n");
        printf("Proceed with any of the functions: \n");
        printf("a : to add user's order to the order book\n");
        printf("b : to check user's orders in the order book\n");
        printf("c : to check the current status of the order book\n");
        printf("d : view market statistics\n");
        printf("e : match orders\n");
        printf("f : cancel order\n");          
        printf("q : quit\n");

        printf("enter the code :");
        scanf(" %c",&c);

        switch(c){  //depending on what c the value of c is
            case 'a':
            {
                int id, qty;    
                float price;    
                char user[MAX_LENGTH];  

                printf("enter order id: ");     //reads order id
                scanf(" %d",&id);
                printf("\nenter user name: ");  //reads user name
                scanf(" %s",user);              // removed & for array
                printf("\nenter the price: ");  //reads price
                scanf(" %f",&price);
                printf("\nenter quantity: ");   //reads quantity
                scanf(" %d",&qty);

                Order* neworder = createorder(id,user,price,qty);
                addOrders(&stocks_list,neworder);  
                break;
            }
            case 'b':
            {
                char curr_user[MAX_LENGTH];
                printf("enter username : ");
                scanf("%s", curr_user);     //takes username as input (no &)
                getUserOrders(&stocks_list,curr_user);      //calls getUserOrders() -> prints all buy + sell orders made by that user
                break;
            }
            case 'c':
            {
                printf("The Order Book status : ");
                getOrderBook(&stocks_list);
                break;
            }
            case 'd':
            {
                updatemarketstates(&stocks_list);   //market statistics
                break;
            }
            case 'e':
            {
                matchOrders(&stocks_list);
                break;
            }
            case 'f':
            {
                int orderid;
                char username[MAX_LENGTH];
                printf("enter the orderid: ");
                scanf("%d",&orderid);   //order id input
                printf("enter username:" );
                scanf("%s", username);  //username input (no &)
                cancelOrder(&stocks_list,orderid,username);
                break;
            }
            case 'q':
            {
                printf("Exiting the program.....");
                return 0;
            }
            default: 
            {
                printf("\nInvalid choice!!\n"); //if user types something not in a b c d e f q
            }
        }
    }
}