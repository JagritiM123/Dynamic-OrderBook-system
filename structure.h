#define MAX_ID 15
#define MAX_LENGTH 100
//these are constructs defined by the preprocessor
#include<time.h>    //used for timestamps

typedef struct Order    //this defines a structure representing one order in the market 
{
    int order_id;
    char user[MAX_LENGTH];
    float price;
    char buy_or_sell;    // 'b' if user wants to buy, 's' if user wants to sell
    time_t time_stamps;     //holds the time when the orders were created
    int quantity;
    struct Order* prev; 
    struct Order* next;
    /*
    represents the previous and next order representing the doubly linked list data structure
    */
}Order;

typedef struct bstnode {    //defines a bst where it is used to group orders by price
    float price;    //this bst node represents all orders of this price
    Order* orders;  //linked list of orders with the same price
    struct bstnode* left;   //all prices less than this node go to the left
    struct bstnode* right;  //all prices more than this node go to the right
}bstnode;

typedef struct order_list {     //this represents the enitre market order book
    Order* buyorders;   //pointer to the first order in the sorted buy orders list
    Order* sellorders;  //pointer to the first order in the sorted sell orders list
    bstnode* buybst;    //bst that stores buy orders grouped by price
    bstnode* sellbst;   //bst that stores sell orders grouped by price
    int buycount;
    int sellcount;
    int trades;
}order_list;

//functions prototypes - all functions are implimented in order.c
void initializing_OrderBook(order_list* orderBook);
Order* createorder(int orderid, char user_name[], float price, int quantity);
bstnode* insertbst(bstnode* root, Order* order);
void displaybst(bstnode* root);
void addOrders(order_list* orderBook, Order* order);
void getUserOrders(order_list* OrderBook, char username[]);
void getOrderBook(order_list* OrderBook);
bstnode* findmax(bstnode* root);
bstnode* findmin(bstnode* root);
int calculate_total_quantity(Order* orders);
void executeTrade(Order** buyorders, Order** sellorders, int tradeQty);
void removeOrder(Order** head, Order* order);
bstnode* deleteBST(bstnode* root, float price, order_list* orderBook);
void matchOrders( order_list* orderBook);
void updatemarketstates(order_list* orderBook);
int cancelOrder(order_list* orderBook, int order_id, char username[]);
