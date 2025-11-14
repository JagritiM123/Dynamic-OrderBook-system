#include<stdio.h>
#include"structure.h"
#include<string.h>
#include<stdlib.h>

/*
initializing the count in the orders list for both buy and sell to be zero
*/
void initializing_OrderBook(order_list* orderBook)
{
    orderBook->buycount=0;
    orderBook->sellcount=0;  
    orderBook->buyorders=NULL;
    orderBook->sellorders=NULL;
    orderBook->buybst=NULL;
    orderBook->sellbst=NULL;
}
/*
resets an order_list to an empty state:
    counts(buy/sell) set to 0
    head pointers for buy/sell linked lists set to null
    bst roots set to null

this must be initialized before using the orderBook to avoid reading uninitialized pointers/counters
*/

//creating orders 
Order* createorder(int orderid, char user_name[], float price, int quantity)
{
    Order* neworder = malloc(sizeof(Order));
    if(neworder == NULL) 
    {
        printf("Memory Unavailable");
        exit(1);
    }
    neworder->order_id = orderid;   //stores the received orderid into the struct's order_id field
    strcpy(neworder->user,user_name);   //copies the string input from user_name to the struct field user
    neworder->price = price;
    neworder->quantity = quantity;

    //initializing prv and next 
    neworder->prev = NULL;
    neworder->next = NULL;
    /*
    sets the neworder as a standalone node for now
    important to avoid garbage pointer values
    */

    //stores the current timestamp
    neworder->time_stamps = time(NULL);
    /*
    time(NULL) returns the current timestamp -> we are basically setting it to the current time
    stores the order creation time inside the struct
    */

    return neworder;
}

bstnode* insertbst(bstnode* root, Order* order)    
{
    if (root == NULL)   
    {
        bstnode* newnode = malloc(sizeof(bstnode));
        if (newnode == NULL)
        {
            printf("Memory Unavailable");
            exit(1);
        }
        newnode -> price = order -> price;
        newnode -> orders = order;
        
        newnode -> left = NULL;
        newnode -> right = NULL;
        //new bst has no children

        return newnode;
    }

    if (order->price < root->price) 
    //inserting in the left subtree -> recurively insert into subtree where order price is lesser than current node price
    {
        root->left=insertbst(root->left, order);
    }
    else if(order->price > root->price) 
    //similar to left sub-tree insertion (previous test case)
    {
        root->right =insertbst(root->right,order);
    }
    else    
    //this case is where we append price to a linked list containing an existing same price node in order (appends to the tail)-> no need of new BST
    {
        Order* temp = root->orders;
        while(temp->next!=NULL)
        {
            temp=temp->next;    
        }
        //doubly linked list
        temp->next = order;
        order->prev=temp;
    }
    return root;    //RECURSIVE CALL TAKES PLACE
}

void addOrders(order_list* orderBook, Order* order)
{
    char key;
    printf("want to buy a stock / sell a stock \n");   
    printf("(for buy enter 'b' ; for sell enter 's') \n");
    scanf(" %c",&key);

    //initializing linked pointers
    order->buy_or_sell = key;
    order->prev = NULL;
    order->next = NULL;

    if(key == 'b')
    {
        if(orderBook->buyorders == NULL)
        {
            orderBook->buyorders = order;   //inserting new head
        }
        else    
        {
            Order* temp = orderBook->buyorders; //temp is now used to traverse the buy list

            //priority rule -> highest price or if price is same then earliest timestamp is taken under consideration
            while(temp != NULL &&
                 (temp->price > order->price ||
                 (temp->price == order->price &&
                  temp->time_stamps < order->time_stamps)))
            {
                temp = temp->next;  
            }
            if(temp == orderBook->buyorders)    //inserting at head position
            {
                order->next = temp;
                temp->prev = order;
                orderBook->buyorders = order;
            }
            else if(temp == NULL)   //inserting at tail position
            {
                Order* tail = orderBook->buyorders;
                while(tail->next != NULL) tail = tail->next;

                tail->next = order;
                order->prev = tail;
            }
            else
            { 
                //perfect DLL insertion
                order->next = temp;
                order->prev = temp->prev;
                temp->prev->next = order;
                temp->prev = order;
            }
        }
        orderBook->buycount++;
        orderBook->buybst = insertbst(orderBook->buybst, order);
        printf("\nBuy orders added!!\n");
    }
    else if(key == 's')
    {
        if(orderBook->sellorders == NULL)
        {
            orderBook->sellorders = order;
        }
        else
        {
            Order* temp = orderBook->sellorders;

            while(temp != NULL &&
                 (temp->price < order->price ||
                 (temp->price == order->price &&
                  temp->time_stamps < order->time_stamps)))
            //priority rule -> highest price or if price is same then earliest timestamp is taken under consideration
            {
                temp = temp->next;
            }
            if(temp == orderBook->sellorders)
            {
                order->next = temp;
                temp->prev = order;
                orderBook->sellorders = order;
            }
            else if(temp == NULL)
            {
                Order* tail = orderBook->sellorders;
                while(tail->next != NULL) tail = tail->next;

                tail->next = order;
                order->prev = tail;
            }
            else
            {
                order->next = temp;
                order->prev = temp->prev;
                temp->prev->next = order;
                temp->prev = order;
            }
        }
        orderBook->sellcount++;
        orderBook->sellbst = insertbst(orderBook->sellbst, order);
        printf("\nSell orders added!!\n");
    }
    else
    {
        printf("choice not valid... please enter again");
        free(order);
    }
}

void getUserOrders(order_list* OrderBook,char username[])
{
    Order* user_orders[MAX_LENGTH];   //array of order pointers
    int order_count = 0;    //to check the number of orders made by particular user
    Order* temp1 = OrderBook->buyorders;  //traversal in BuyOrders
    while(temp1!=NULL)
    {
        if(strcmp(temp1->user,username) == 0)
        {
            if(order_count < MAX_LENGTH)
            {
                char time_str[20];
                strftime(time_str,sizeof(time_str),"%H:%M:%S",localtime(&temp1->time_stamps));
                /*
                string format time -> formats date and time into a string buffer
                format is basically hh:mm:ss
                */
                user_orders[order_count] = temp1;
                order_count++;
            }
        }
        temp1=temp1->next;
    }
    Order* temp2 = OrderBook->sellorders;
    while(temp2!=NULL)
    {
        if(strcmp(temp2->user, username) == 0)
        {
            if(order_count < MAX_LENGTH)
            {
                char time_str[20];
                strftime(time_str,sizeof(time_str),"%H:%M:%S",localtime(&temp2->time_stamps));
                user_orders[order_count] = temp2;
                order_count++;
            }
        }
        temp2 = temp2->next;
    }
    printf("Your current Orders(%s):\n", username);     //prints username
    for (int i = 0; i < order_count; i++)   //loops through all the matching orders depending on the order count
    {
        char time_str[20];  //to hold the timestamp
        if (strftime(time_str, sizeof(time_str), "%H:%M:%S", 
                    localtime(&user_orders[i]->time_stamps)) == 0)
        {
            //DISCLAIMER: this is only if strftime fails
            strncpy(time_str, "00:00:00", sizeof(time_str));    //if strftime fails (rarely) then manually add the "00:00:00" timestamp so that printing does not break
            time_str[sizeof(time_str) - 1] = '\0';  //add terminator in the end
        }
        printf("Order ID: %d | Price: %.2f | Quantity: %d | Type: %c | time: %s\n",
            user_orders[i]->order_id,
            user_orders[i]->price,
            user_orders[i]->quantity,
            user_orders[i]->buy_or_sell,
            time_str);
    }
    if (order_count == 0)
    {
        printf("Sorry No orders found for user : %s\n", username);
    }
}

void getOrderBook(order_list* OrderBook)
{
    Order* temp1 = OrderBook->buyorders;    //head of buy list
    printf("BUY ORDERS:-\n");
    printf("   Order Id   User       price       quantity       time\n");
    while(temp1 != NULL)
    {
        char time_str[20];
        strftime(time_str,sizeof(time_str),"%H:%M:%S",localtime(&temp1->time_stamps));
        printf("       %d       %s       %.2f       %d              %s\n",
            temp1->order_id,temp1->user,temp1->price,temp1->quantity,time_str);
        temp1 = temp1->next;
    }

    Order* temp2 = OrderBook->sellorders;   //head of sell list
    printf("\nSELL ORDERS:-\n");
    printf("   Order Id   User       price       quantity       time\n");
    while(temp2 != NULL)
    {
        char time_str[20];
        strftime(time_str,sizeof(time_str),"%H:%M:%S",localtime(&temp2->time_stamps));
        printf("       %d       %s       %.2f       %d             %s\n",
            temp2->order_id,temp2->user,temp2->price,temp2->quantity,time_str);
        temp2 = temp2->next;
    }
}

bstnode* findmax(bstnode* root)    
{
    if (root == NULL) return NULL;
    while (root->right!=NULL)
    {
        root = root->right;
    }
    return root;
}

bstnode* findmin(bstnode* root)     
{
    if (root == NULL) return NULL;
    while (root->left!=NULL)
    {
        root = root->left;
    }
    return root;
}

int calculate_total_quantity(Order* orders) //total quantity in orders
{ 
    int total = 0;
    Order* temp = orders;   //temp variable if for tranversal to calculate total quantity
    while (temp!=NULL){
        total += temp->quantity;
        temp = temp->next;
    }
    return total;
}
   
void executeTrade(Order** buyHead, Order** sellHead, int tradeQty)
{
    int remaining = tradeQty;
    Order* buytemp = *buyHead; 
    Order* selltemp = *sellHead;    

    while (buytemp != NULL && selltemp != NULL && remaining > 0)
    {
        int matchQty = buytemp->quantity;
        if (selltemp->quantity < matchQty)
        {
            matchQty = selltemp->quantity;
        }
        if (remaining < matchQty)
        {
            matchQty = remaining;
        }

        // Reduce both BUY and SELL quantities by the matched amount.
        buytemp->quantity -= matchQty;
        selltemp->quantity -= matchQty;
        remaining -= matchQty;  // reduce the leftover trade quantity after the match

        if (buytemp->quantity == 0)
        {
            // if buy order is fully matched (quantity becomes 0) -> it sets the quantity to 0 → this means the order is complete
            Order* nextBuy = buytemp->next;
            removeOrder(buyHead, buytemp);  // removeorder unlinks & frees the node
            buytemp = nextBuy;              // moving to next buy order
        }

        if (selltemp->quantity == 0)
        {
            // if sell order is fully matched (quantity becomes 0) -> same logic as BUY, the order is completely satisfied.
            Order* nextSell = selltemp->next;
            removeOrder(sellHead, selltemp);
            selltemp = nextSell;           // moving to next sell order
        }
    }
}

void removeOrder(Order** head, Order* order) 
{
    if (*head == NULL || order == NULL) return; // nothing to remove
    if (*head == order) 
    {
        *head = order->next;
    }
    if (order->next != NULL) 
    {
        order->next->prev = order->prev;
    }
    if (order->prev != NULL) 
    {
        order->prev->next = order->next;
    }

    order->prev = NULL;
    order->next = NULL;

    free(order); // delete node
}

bstnode* deleteBST(bstnode* root, float price, order_list* orderBook) 
{
    if (root == NULL) return root;
    if (price < root->price) 
    {
        root->left = deleteBST(root->left,price,orderBook);     //recurse left 
    } 
    else if (price>root->price) 
    {
        root->right=deleteBST(root->right,price,orderBook);     //recurse right
    } 
    else 
    {
        // Node with the target price found
        if (root->left == NULL) 
        {
            bstnode* temp = root->right;    
            //so basically here temp is declred as the right child of the node (node doesnt have a left child) and this right child replaces the node after deletion
            free(root);
            return temp;
        } 
        else if (root->right==NULL) 
        {
            bstnode* temp=root->left;
            //so basically here temp is declred as the left child of the node (node doesnt have a right child) and this left child replaces the node after deletion
            free(root);
            return temp;
        } 
        else 
        {
            bstnode* temp = findmin(root->right); //here we find the inorder successor
            root->price = temp->price;  //copy successor price to root
            root->orders = temp->orders;    //copy successor orderd to root
            root->right = deleteBST(root->right,temp->price,orderBook); //here we are basically replacing the right child of the root with the remaining tree after deletion
        }
    }
    return root;
}

void matchOrders(order_list *orderBook)
{
    int trade_count = 0;    //count trades executed during this call
    while(orderBook->buyorders && orderBook->sellorders) 
    //orderBook->buyorders is assumed to be pointing to best buy price and orderBook->sellorders is assumed to be pointing to best sell price
    {
        Order* bestbuy = orderBook->buyorders; 
        Order* bestsell = orderBook->sellorders;    

        if(bestbuy->price >= bestsell->price)
        {
            int trade_qty = (bestbuy->quantity < bestsell->quantity) ? bestbuy->quantity : bestsell->quantity;
            //trade quantity is the minimum of the available quantities (partial fills allowed)

            printf("\n trade initiated for %d units at %.1f price \n",trade_qty,bestbuy->price);

            bestbuy->quantity -= trade_qty;
            bestsell->quantity -= trade_qty;

            // save next pointers BEFORE removal to prevent use-after-free
            Order* next_buy  = bestbuy->next;
            Order* next_sell = bestsell->next;

            // Remove fully executed orders
            if (bestbuy->quantity <= 0) 
            {
                removeOrder(&orderBook->buyorders, bestbuy);
                orderBook->buycount--;
            }
            if (bestsell->quantity <= 0) 
            {
                removeOrder(&orderBook->sellorders, bestsell);
                orderBook->sellcount--;
            }
            trade_count++;
        }
        else break;
        orderBook->trades = trade_count;
        printf("total trades executed for this matching: %d",trade_count);
    }
}

void updatemarketstates(order_list* orderBook)  //summary of the current market stats
{
    bstnode* buynode = orderBook->buybst;
    bstnode* sellnode = orderBook->sellbst;
    while(buynode && buynode->right) 
    {
        buynode = buynode->right;   //highest buy price
    }
    while(sellnode && sellnode->left) 
    {
        sellnode = sellnode->left;  //lowest sell price
    }
    printf("\nMarket State\n");
    if(buynode) 
    {
        printf("highest buy price: %f\n",buynode->price);
    }
    else 
    {
        printf("No buy orders.\n");
    }
    if (sellnode) 
    {
        printf("best ask: %f\n",sellnode->price);
    }
    else 
    {
        printf("No sell orders.\n");
    }
    printf("total number of trades in total: %d\n",orderBook->trades);
    printf("total buy orders: %d\n",orderBook->buycount);
    printf("total sell orders: %d\n",orderBook->sellcount);
}

int cancelOrder(order_list* orderBook, int id, char username[])
{
    char key;
    printf("want to delete buy or sell??\n");
    printf("buy: b | sell: s\n");
    scanf(" %c",&key);
    if(key == 'b')
    {
        Order* temp1 = orderBook->buyorders;
        while(temp1 != NULL)
        {
            Order* nxt = temp1->next;
            if(temp1->order_id == id && strcmp(temp1->user, username) == 0)    //if both match
            {
                removeOrder(&orderBook->buyorders, temp1);
                free(temp1); 
                orderBook->buycount--;           //decrement buy count
                printf("\nBuy order %d for username:%s cancelled!! \n", id, username);
                printf("\nBuy count: %d\n", orderBook->buycount);
                return 1;
            }
            temp1 = nxt;
        }
    }
    else if (key == 's')
    {
        Order* temp2 = orderBook->sellorders;
        while(temp2 != NULL)
        {
            Order* nxt = temp2->next;
            if(temp2->order_id == id && strcmp(temp2->user, username) == 0)    //if both match
            {
                removeOrder(&orderBook->sellorders, temp2);
                free(temp2);  
                orderBook->sellcount--;          //decrement sell count
                printf("\nsell order %d for username:%s cancelled!! \n", id, username);
                return 1;
            }
            temp2 = nxt;
        }
    }
    else
    {
        printf("invalid case!!\n");
        return 0;
    }
    printf("Order not found.\n");
    return 0;
}

