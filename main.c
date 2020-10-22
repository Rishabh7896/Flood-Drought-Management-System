/******************************************************************************
                                DSA COURSE PROJECT
Team Number: 1(one)
Team Member 1: Rishabh Gautam
Team Member 2: Raksha Burli
Team Member 3: Sakshi Jha
Team Member 4: Rohan Kumar
Division: C(3rd semester - CSE dept)
University:  KLE Technological University
******************************************************************************/


/***-----------------------------------------------------------------------***/
                      /** Global Declaration Space **/

///Standard library files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include <sys/stat.h>

///Defined constants
#define SUCCESS 1
#define FAILURE 0
#define MAX 500
#define CAPACITY 30 //there are 3 tanks each of height 10ft

///structure to store missing person details
struct mperson
{
    char name[25];
    int age;
    int time;   //total time(days) since missing
    char remarks[50];
    struct mperson *left;
    struct mperson *right;
};

///structure to store locality details
struct locality
{
    char localityname[15];
    int localityid;
    int housecount;
    int popcount;
    int altitude;
    struct locality *nextlocality;
};

///structure to store city details
struct city
{
    char cityname[15];
    int cityid;
    float tankv;
    struct locality *inlocality;
    struct city *nextcity;
};

///structure to store state details
struct state
{
    char statename[15];
    int stateid;
    float graph[MAX][MAX];
    float waternetwork[15][15];
    struct city *incity;
    struct state *nextstate;
};
typedef struct state STATE;

///Global Variables
STATE *head;
struct mperson *root;
int network=SUCCESS;

///Function prototypes
void welcome();
void initialize();
int load();
int file_empty_check(char *filename);
struct state *mem_state();
struct city *mem_city();
struct locality *mem_locality();
struct mperson *mem_person();
int add_loc_rear(struct city *tempc,struct locality *templ);
int add_city_rear(struct state *temps,struct city *tempc);
int add_state_rear(struct state *temps);
void menu();
void dump();
void print_menu();
void print();
int print_state();
int print_city();
int print_locality();
void pop_menu();
void pop_count();
int count_state();
int count_city();
int count_locality();
int create_graph();
void get_water_level();
void connected();
void find_pat();
int pattern(char txt[15],char pat[15]);
void disaster_menu();
int drought();
int create_waternetwork();
void all_pair_shortestpath();
int flood();
int allvisited(int visited[450]);
void missing_person();
void insertbst(struct mperson *p);
void inorder(struct mperson *m);

/***-----------------------------------------------------------------------***/

///Function:     main
///Description:  calls all necessary functions
///Input param:  NIL
///Return Type:  integer indicating the status
int main()
{
    welcome();
    initialize();
    int status;
    status=load();
    if(status==SUCCESS)
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~ Data Loaded Successfully ~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    else
    {
        printf("\n--------- Load Error --------\n");
        printf("---- Program Terminating ----\n");
        exit(0);
    }
    status=create_graph();
    if(status!=SUCCESS)
    {
        printf("\nDo you want to try creating graph again(Y|N): ");
        char ch;
        scanf("%c",&ch);
        if(ch=='Y')
        {
            status=create_graph();
            if(status!=SUCCESS)
            {
                printf("\n----- Program Terminating -----\n");
                exit(0);
            }
        }
        else
        {
            printf("\n----- Program Terminating -----\n");
            exit(0);
        }
    }
    else
        printf("~~~~~~~~~~~~~~~~~~~~~~~ Graph Created Successfully ~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    status=create_waternetwork();
    if(status!=SUCCESS)
    {
        printf("\nDo you want to try creating network again(Y|N): ");
        char ch;
        scanf("%c",&ch);
        if(ch=='Y')
        {
            status=create_waternetwork();
            if(status!=SUCCESS)
            {
                printf("\n----- You won't be able to manage disasters -----\n");
                network=FAILURE;
            }
        }
        else
        {
            printf("\n----------------- You won't be able to manage disasters -----------------\n\n");
            network=FAILURE;
        }
    }
    else
        printf("~~~~~~~~~~~~~~~~~~~ Water Network Created Successfully ~~~~~~~~~~~~~~~~~~~\n\n");
    menu();

    return 0;
}

/***-----------------------------------------------------------------------***/

///Function:     welcome
///Description:  displays welcome message
void welcome()
{
    printf("*-------------------------------------------------------------------------*\n");
    printf("*                  WELCOME TO Disaster Management Program                 *\n");
    printf("*-------------------------------------------------------------------------*\n");
    printf("        This program helps you evacuate a city when there is flood         \n");
}

///Function:     initialize
///Description:  initializes the head pointer to nullptr
void initialize()
{
    head=NULL;
    root=NULL;
}


///Function:    mem_state
///Description: dynamically allocates the memory for
///             struct state type
struct state *mem_state()
{
    struct state *p;
    p=(struct state *)malloc(sizeof(struct state));
    if(p==NULL)
    {
        printf("\n---- Memory allocation Error ----\n");
        printf("---- Program Terminating ----\n");
        exit(0);
    }
    return p;
}

///Function:    mem_city
///Description: dynamically allocates the memory for
///             struct city type
struct city *mem_city()
{
    struct city *p;
    p=(struct city *)malloc(sizeof(struct city));
    if(p==NULL)
    {
        printf("\n---- Memory allocation Error ----\n");
        printf("---- Program Terminating ----\n");
        exit(0);
    }
    return p;
}

///Function:    mem_locality
///Description: dynamically allocates the memory for
///             struct locality type
struct locality *mem_locality()
{
    struct locality *p;
    p=(struct locality *)malloc(sizeof(struct locality));
    if(p==NULL)
    {
        printf("\n---- Memory allocation Error ----\n");
        printf("---- Program Terminating ----\n");
        exit(0);
    }
    return p;
}

///Function:     add_loc_rear
///Description:  adds the locality details at the end
int add_loc_rear(struct city *tempc,struct locality *templ)
{
    struct locality *p;
    p=tempc->inlocality;
    if(p==NULL)
    {
        tempc->inlocality=templ;
        return SUCCESS;
    }
    else
    {
        while(p->nextlocality!=NULL)
            p=p->nextlocality;
        p->nextlocality=templ;
        return SUCCESS;
    }
    return FAILURE;
}

///Function:     add_city_rear
///Description:  adds the city details at the end
int add_city_rear(struct state *temps,struct city *tempc)
{
    struct city *p;
    p=temps->incity;
    if(p==NULL)
    {
        temps->incity=tempc;
        return SUCCESS;
    }
    else
    {
        while(p->nextcity!=NULL)
            p=p->nextcity;
        p->nextcity=tempc;
        return SUCCESS;
    }
    return FAILURE;
}

///Function:     add_state_rear
///Description:  adds the state details at the end
int add_state_rear(struct state *temps)
{
    if(head==NULL)
    {
        head=temps;
        return SUCCESS;
    }
    else
    {
        struct state *p;
        p=head;
        while(p->nextstate!=NULL)
            p=p->nextstate;
        p->nextstate=temps;
        return SUCCESS;
    }
    return FAILURE;
}

///Function:     file_empty_check
///Description:  Check whether file has content or not
int file_empty_check(char *filename)
{
    FILE* fp= fopen(filename,"r");
    long fsize = 0;
    if(fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        fsize= ftell(fp);
    }
    char message_failure[40];
    strcpy(message_failure,filename);
    strcat(message_failure, " is empty");
    if(fsize==0)
    {
        printf("\n---- %s ----\n",message_failure);
        return FAILURE;
    }
    else
        return SUCCESS;
}

///Function:     load
///Description:  creates a linked list of the given data
/// we have assumed that there are 15 cities in a state
/// and each city has 30 localities.(same in our data set)
///***Note*** : we had assumed that the data present in the
/// file is fully correct and there is no need
/// to clean the data
int load()
{
    FILE *fp;
    fp=fopen("data.txt","r");
    if(fp==NULL)
    {
        printf("\n---- No Such file Exits ----\n");
        return FAILURE;
    }
    if(file_empty_check("data.txt")!=1)
        return FAILURE;
    struct state *temps;
    struct city *tempc;
    struct locality *templ;
    while(!feof(fp))
    {
        temps=mem_state();
        fscanf(fp,"%s",temps->statename);
        fscanf(fp,"%d",&temps->stateid);
        temps->nextstate=NULL;
        temps->incity=NULL;
        for(int i=0;i<15;i++)
        {
            tempc=mem_city();
            fscanf(fp,"%s",tempc->cityname);
            fscanf(fp,"%d",&tempc->cityid);
            tempc->tankv=10; //we had assumed that there is 10ft of water in every city
            tempc->nextcity=NULL;
            tempc->inlocality=NULL;
            for(int j=0;j<30;j++)
            {
                templ=mem_locality();
                fscanf(fp,"%s",templ->localityname);
                fscanf(fp,"%d",&templ->localityid);
                fscanf(fp,"%d",&templ->housecount);
                fscanf(fp,"%d",&templ->popcount);
                fscanf(fp,"%d",&templ->altitude);
                templ->nextlocality=NULL;
                int status=add_loc_rear(tempc,templ);
                if(status==FAILURE)
                {
                    fclose(fp);
                    return FAILURE;
                }
            }
            int status=add_city_rear(temps,tempc);
            if(status==FAILURE)
            {
                fclose(fp);
                return FAILURE;
            }
        }
        int status=add_state_rear(temps);
        if(status==FAILURE)
        {
            fclose(fp);
            return FAILURE;
        }
    }
    fclose(fp);
    return SUCCESS;
}

///Function:     print
///Description:  prints all the loaded data
void print()
{
    struct state *s;
    s=head;
    if(s==NULL)
    {
        printf("\n--- List Empty ----\n");
        return ;
    }
    struct city *c;
    struct locality *l;
    printf("(printing data.....)\n\n");
    printf("---------------------------------- Data ----------------------------------\n");
    while(s!=NULL)
    {
        printf("State Name:%s\n",s->statename);
        printf("State ID: %d\n",s->stateid);
        printf("===========================================================================\n");
        c=s->incity;
        while(c!=NULL)
        {
            printf("City Name:%s\n",c->cityname);
            printf("City ID: %d\n",c->cityid);
            printf("Water Availability: %.2f ft\n",c->tankv);
            printf("---------------------------------------------------------------------------\n");
            printf("| Locality Name | Locality ID | House Count | Population Count | Altitude |\n");
            printf("---------------------------------------------------------------------------\n");
            l=c->inlocality;
            while(l!=NULL)
            {
                printf(" %s",l->localityname);
                for(int i=0;i<20-strlen(l->localityname);i++)
                    printf(" ");
                printf("%d         ",l->localityid);
                printf("%d            ",l->housecount);
                printf("%d            ",l->popcount);
                if(l->popcount<10000)
                    printf(" ");
                printf("%d\n",l->altitude);
                l=l->nextlocality;
            }
            printf("===========================================================================\n");
            c=c->nextcity;
        }
        s=s->nextstate;
    }
    printf("\n");
}

///Function:     menu
///Description:  prints the program menu options
void menu()
{
    int ch;
    while(1)
    {
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("|                    MENU                    |\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("0-> Exit\n");
        printf("1-> Display and Search\n");
        printf("2-> Population & House Count\n");
        printf("3-> Get water level\n");
        printf("4-> Get the connected localities\n");
        printf("5-> Find a Pattern in data\n");
        printf("6-> Update the graph with original value\n");
        printf("7-> DISASTER MODE\n");
        printf("8-> Report missing person\n");
        printf("9-> Display Missing person details\n");
        printf("Enter your Choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 0: dump();
                    break;

            case 1: print_menu();
                    break;

            case 2: pop_menu();
                    break;

            case 3: get_water_level();
                    break;

            case 4: connected();
                    break;

            case 5: find_pat();
                    break;

            case 6: create_graph();
                    printf("\n------- Graph Updated Successfully -------\n\n");
                    break;

            case 7: disaster_menu();
                    break;

            case 8: missing_person();
                    break;

            case 9: if(root==NULL)
                        printf("\n----- No missing person -----\n\n");
                    else
                    {
                        printf("\nMissing Person details:\n");
                        printf("-----------------------------------\n");
                        inorder(root);
                        printf("\n");
                    }
                    break;

            default: printf("\a");
                     printf("\n---- Invalid Choice ----\n\n");
                     break;
        }
    }
}

///Function:     dump
///Description:  dumps all the data and frees up the allocated memories
void dump()
{
    struct state *s,*ts;
    struct city *c,*tc;
    struct locality *l,*tl;
    s=head;
    head=NULL;
    while(s!=NULL)
    {
        ts=s;
        s=s->nextstate;
        c=ts->incity;
        while(c!=NULL)
        {
            tc=c;
            c=c->nextcity;
            l=tc->inlocality;
            while(l!=NULL)
            {
                tl=l;
                l=l->nextlocality;
                free(tl);
            }
            free(tc);
        }
        free(ts);
    }
    printf("\n---- Data Dumped Successfully ----\n");
    exit(0);
}

///Function:     print_menu
///Description:  displays option of what do you want to display
void print_menu()
{
    int ch,status;
    char ch1;
    while(1)
    {
        printf("\n~~~~~~~~~~ PRINT MENU ~~~~~~~~~~\n");
        printf("0-> Return to main menu\n");
        printf("1-> Display whole data\n");
        printf("2-> Display data of a particular state\n");
        printf("3-> Display data of a particular city\n");
        printf("4-> Display data of a particular locality\n");
        printf("Enter your choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 0: printf("\n---- Process Terminated ----\n\n");
                    return ;

            case 1: printf("\nPrinting the data is a time taking process\n");
                    printf("Do you still want to continue(Y/N): ");
                    scanf("%*c%c",&ch1);
                    if(ch1=='N')
                        printf("\n---- Process Terminated ----\n\n");
                    else if(ch1=='Y')
                        print();
                    else
                    {
                        printf("\n---- Invalid Choice ----\n");
                        break;
                    }
                    return;

            case 2: status=print_state();
                    if(status!=1)
                        break;
                    else
                        return;

            case 3: status=print_city();
                    if(status!=1)
                        break;
                    else
                        return;

            case 4: status=print_locality();
                    if(status!=1)
                        break;
                    else
                        return;

            default: printf("\n---- Invalid Print Choice ----\n");
                     break;
        }
    }
}

///Function:    print_state
///Description: prints the data of a particular state
int print_state()
{
    int ch,id;
    char name[15];
    struct state *s;
    struct city *c;
    struct locality *l;
    s=head;
    while(1)
    {
        printf("\n~~~~~~~ PRINT STATE MENU ~~~~~~~\n");
        printf("0-> return to print menu\n");
        printf("How to do you want search state?\n");
        printf("1-> by state name\n");
        printf("2-> by state ID\n");
        printf("Enter your choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 0: printf("\n---- Process Terminated ----\n");
                    return FAILURE;

            case 1: printf("\nEnter the state name to search: ");
                    scanf("%s",name);
                    while(s!=NULL)
                    {
                        if(strcasecmp(s->statename,name)==0)
                            break;
                        s=s->nextstate;
                    }
                    if(s==NULL)
                        printf("\n---- No Matching states Found ----\n\n");
                    else
                    {
                        printf("---------------------------------- Data ----------------------------------\n");
                        printf("State Name:%s\n",s->statename);
                        printf("State ID: %d\n",s->stateid);
                        printf("===========================================================================\n");
                        c=s->incity;
                        while(c!=NULL)
                        {
                            printf("City Name:%s\n",c->cityname);
                            printf("City ID: %d\n",c->cityid);
                            printf("Water Availability: %.2f ft\n",c->tankv);
                            printf("---------------------------------------------------------------------------\n");
                            printf("| Locality Name | Locality ID | House Count | Population Count | Altitude |\n");
                            printf("---------------------------------------------------------------------------\n");
                            l=c->inlocality;
                            while(l!=NULL)
                            {
                                printf(" %s",l->localityname);
                                for(int i=0;i<20-strlen(l->localityname);i++)
                                    printf(" ");
                                printf("%d         ",l->localityid);
                                printf("%d            ",l->housecount);
                                printf("%d            ",l->popcount);
                                if(l->popcount<10000)
                                    printf(" ");
                                printf("%d\n",l->altitude);
                                l=l->nextlocality;
                            }
                            printf("===========================================================================\n");
                            c=c->nextcity;
                        }
                        printf("\n");
                    }
                    return SUCCESS;

            case 2: printf("\nEnter the state ID to search: ");
                    scanf("%d",&id);
                    while(s!=NULL)
                    {
                        if(s->stateid==id)
                            break;
                        s=s->nextstate;
                    }
                    if(s==NULL)
                        printf("\n---- No Matching states Found ----\n\n");
                    else
                    {
                        printf("---------------------------------- Data ----------------------------------\n");
                        printf("State Name:%s\n",s->statename);
                        printf("State ID: %d\n",s->stateid);
                        printf("===========================================================================\n");
                        c=s->incity;
                        while(c!=NULL)
                        {
                            printf("City Name:%s\n",c->cityname);
                            printf("City ID: %d\n",c->cityid);
                            printf("Water Availability: %.2f ft\n",c->tankv);
                            printf("---------------------------------------------------------------------------\n");
                            printf("| Locality Name | Locality ID | House Count | Population Count | Altitude |\n");
                            printf("---------------------------------------------------------------------------\n");
                            l=c->inlocality;
                            while(l!=NULL)
                            {
                                printf(" %s",l->localityname);
                                for(int i=0;i<20-strlen(l->localityname);i++)
                                    printf(" ");
                                printf("%d         ",l->localityid);
                                printf("%d            ",l->housecount);
                                printf("%d            ",l->popcount);
                                if(l->popcount<10000)
                                    printf(" ");
                                printf("%d\n",l->altitude);
                                l=l->nextlocality;
                            }
                            printf("===========================================================================\n");
                            c=c->nextcity;
                        }
                        printf("\n");
                    }
                    return SUCCESS;

            default: printf("\n---- Invalid Choice ----\n");
                     break;
        }
    }
}

///Function:    print_city
///Description: prints the data of a particular city
int print_city()
{
    int ch,id;
    char name[15];
    struct state *s;
    struct city *c;
    struct locality *l;
    s=head;
    while(1)
    {
        printf("\n~~~~~~~ PRINT CITY MENU ~~~~~~~\n");
        printf("0-> return to print menu\n");
        printf("How to do you want search city?\n");
        printf("1-> by city name\n");
        printf("2-> by city ID\n");
        printf("Enter your choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 0: printf("\n---- Process Terminated ----\n");
                    return FAILURE;

            case 1: printf("\nEnter the city name to search: ");
                    scanf("%s",name);
                    while(s!=NULL)
                    {
                        c=s->incity;
                        while(c!=NULL)
                        {
                            if(strcasecmp(c->cityname,name)==0)
                                break;
                            c=c->nextcity;
                        }
                        if(strcasecmp(c->cityname,name)==0)
                            break;
                        s=s->nextstate;
                    }
                    if(s==NULL)
                        printf("\n---- No Matching City Found ----\n\n");
                    else
                    {
                        printf("\n---------------------------------- Data ----------------------------------\n");
                        printf("City Name:%s\n",c->cityname);
                        printf("City ID: %d\n",c->cityid);
                        printf("Water Availability: %.2f ft\n",c->tankv);
                        printf("===========================================================================\n");
                        printf("State Name:%s\n",s->statename);
                        printf("State ID: %d\n",s->stateid);
                        printf("---------------------------------------------------------------------------\n");
                        printf("| Locality Name | Locality ID | House Count | Population Count | Altitude |\n");
                        printf("---------------------------------------------------------------------------\n");
                        l=c->inlocality;
                        while(l!=NULL)
                        {
                            printf(" %s",l->localityname);
                            for(int i=0;i<20-strlen(l->localityname);i++)
                                printf(" ");
                            printf("%d         ",l->localityid);
                            printf("%d            ",l->housecount);
                            printf("%d            ",l->popcount);
                            if(l->popcount<10000)
                                printf(" ");
                            printf("%d\n",l->altitude);
                            l=l->nextlocality;
                        }
                        printf("===========================================================================\n");
                        printf("\n");
                    }
                    return SUCCESS;

            case 2: printf("\nEnter the city ID to search: ");
                    scanf("%d",&id);
                    while(s!=NULL)
                    {
                        c=s->incity;
                        while(c!=NULL)
                        {
                            if(c->cityid==id)
                                break;
                            c=c->nextcity;
                        }
                        if(c!=NULL && c->cityid==id)
                            break;
                        s=s->nextstate;
                    }
                    if(s==NULL)
                        printf("\n---- No Matching City Found ----\n\n");
                    else
                    {
                        printf("\n---------------------------------- Data ----------------------------------\n");
                        printf("City Name:%s\n",c->cityname);
                        printf("City ID: %d\n",c->cityid);
                        printf("Water Availability: %.2f ft\n",c->tankv);
                        printf("===========================================================================\n");
                        printf("State Name:%s\n",s->statename);
                        printf("State ID: %d\n",s->stateid);
                        printf("---------------------------------------------------------------------------\n");
                        printf("| Locality Name | Locality ID | House Count | Population Count | Altitude |\n");
                        printf("---------------------------------------------------------------------------\n");
                        l=c->inlocality;
                        while(l!=NULL)
                        {
                            printf(" %s",l->localityname);
                            for(int i=0;i<20-strlen(l->localityname);i++)
                                printf(" ");
                            printf("%d         ",l->localityid);
                            printf("%d            ",l->housecount);
                            printf("%d            ",l->popcount);
                            if(l->popcount<10000)
                                printf(" ");
                            printf("%d\n",l->altitude);
                            l=l->nextlocality;
                        }
                        printf("===========================================================================\n");
                        printf("\n");
                    }
                    return SUCCESS;

            default: printf("\n---- Invalid Choice ----\n");
                     break;
        }
    }
}

///Function:    print_locality
///Description: prints the data of a particular locality
int print_locality()
{
    int ch,id;
    char name[15];
    struct state *s;
    struct city *c;
    struct locality *l;
    s=head;
    while(1)
    {
        printf("\n~~~~~~~ PRINT LOCALITY MENU ~~~~~~~\n");
        printf("0-> return to print menu\n");
        printf("How to do you want search locality?\n");
        printf("1-> by locality name\n");
        printf("2-> by locality ID\n");
        printf("Enter your choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 0: printf("\n---- Process Terminated ----\n");
                    return FAILURE;

            case 1: printf("\nEnter the locality name to search: ");
                    scanf("%s",name);
                    while(s!=NULL)
                    {
                        c=s->incity;
                        while(c!=NULL)
                        {
                            l=c->inlocality;
                            while(l!=NULL)
                            {
                                if(strcasecmp(l->localityname,name)==0)
                                    break;
                                l=l->nextlocality;
                            }
                            if(strcasecmp(l->localityname,name)==0)
                                    break;
                            c=c->nextcity;
                        }
                        if(strcasecmp(l->localityname,name)==0)
                                    break;
                        s=s->nextstate;
                    }
                    if(s==NULL)
                        printf("\n---- No Matching Locality Found ----\n\n");
                    else
                    {
                        printf("\n----------------- Data -----------------\n");
                        printf("Locality Name: %s\n",l->localityname);
                        printf("Locality ID: %d\n",l->localityid);
                        printf("State Name:%s\n",s->statename);
                        printf("State ID: %d\n",s->stateid);
                        printf("City Name:%s\n",c->cityname);
                        printf("City ID: %d\n",c->cityid);
                        printf("Water Availability: %.2f ft\n",c->tankv);
                        printf("Locality house count: %d\n",l->housecount);
                        printf("Locality population count: %d\n",l->popcount);
                        printf("Locality Altitude: %d\n",l->altitude);
                        printf("----------------------------------------\n\n");
                    }
                    return SUCCESS;

            case 2: printf("\nEnter the locality ID to search: ");
                    scanf("%d",&id);
                    while(s!=NULL)
                    {
                        c=s->incity;
                        while(c!=NULL)
                        {
                            l=c->inlocality;
                            while(l!=NULL)
                            {
                                if(l->localityid==id)
                                {
                                    printf("\n----------------- Data -----------------\n");
                                    printf("Locality Name: %s\n",l->localityname);
                                    printf("Locality ID: %d\n",l->localityid);
                                    printf("State Name:%s\n",s->statename);
                                    printf("State ID: %d\n",s->stateid);
                                    printf("City Name:%s\n",c->cityname);
                                    printf("City ID: %d\n",c->cityid);
                                    printf("Water Availability: %.2f ft\n",c->tankv);
                                    printf("Locality house count: %d\n",l->housecount);
                                    printf("Locality population count: %d\n",l->popcount);
                                    printf("Locality Altitude: %d\n",l->altitude);
                                    printf("----------------------------------------\n\n");
                                    return SUCCESS;
                                }
                                l=l->nextlocality;
                            }
                            c=c->nextcity;
                        }
                        s=s->nextstate;
                    }
                    printf("\n---- No Matching Locality Found ----\n\n");
                    return SUCCESS;


            default: printf("\n---- Invalid Choice ----\n");
                     break;
        }
    }
}

///Function:     pop_menu
///Description:  gives option of which population user wants.
void pop_menu()
{
    int ch,status;
    char ch1;
    while(1)
    {
        printf("\n~~~~~ POPULATION COUNT MENU ~~~~~\n");
        printf("0-> Return to main menu\n");
        printf("1-> Population of entire data\n");
        printf("2-> Population of a particular state\n");
        printf("3-> Population of a particular city\n");
        printf("4-> Population of a particular locality\n");
        printf("Enter your choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 0: printf("\n---- Process Terminated ----\n\n");
                    return ;

            case 1: printf("\nCounting through entire data is a time taking process\n");
                    printf("Do you still want to continue(Y/N): ");
                    scanf("%*c%c",&ch1);
                    if(ch1=='N')
                        printf("\n---- Process Terminated ----\n\n");
                    else if(ch1=='Y')
                        pop_count();
                    else
                    {
                        printf("\n---- Invalid Choice ----\n");
                        break;
                    }
                    return;

            case 2:status=count_state();
                    if(status!=1)
                        break;
                    else
                        return;

            case 3: status=count_city();
                    if(status!=1)
                        break;
                    else
                        return;

            case 4: status=count_locality();
                    if(status!=1)
                        break;
                    else
                        return;

            default: printf("\n---- Invalid Choice ----\n");
                     break;
        }
    }
}

///Function:     pop_count
///Description:  counts the population of whole data
void pop_count()
{
    struct state *s;
    s=head;
    if(s==NULL)
    {
        printf("\n--- List Empty ----\n");
        return ;
    }
    struct city *c;
    struct locality *l;
    long long int pcount=0,hcount=0;
    while(s!=NULL)
    {
        c=s->incity;
        while(c!=NULL)
        {
            l=c->inlocality;
            while(l!=NULL)
            {
                pcount+=l->popcount;
                hcount+=l->housecount;
                l=l->nextlocality;
            }
            c=c->nextcity;
        }
        s=s->nextstate;
    }
    printf("\nTotal House Count: %lld\n",hcount);
    printf("Total Population Count: %lld\n\n",pcount);
}

///Function:    count_state
///Description: counts the population of the state
int count_state()
{
    struct state *s;
    struct city *c;
    struct locality *l;
    long long int pcount=0,hcount=0;
    s=head;
    int id;
    printf("\nEnter the state ID to count: ");
    scanf("%d",&id);
    while(s!=NULL)
    {
        if(s->stateid==id)
            break;
        s=s->nextstate;
    }
    if(s==NULL)
    {
        printf("\n---- No Matching states Found ----\n");
        return FAILURE;
    }
    else
    {
        c=s->incity;
        while(c!=NULL)
        {
            l=c->inlocality;
            while(l!=NULL)
            {
                pcount+=l->popcount;
                hcount+=l->housecount;
                l=l->nextlocality;
            }
            c=c->nextcity;
        }
    }
    printf("\nState Name: %s\n",s->statename);
    printf("Total House Count: %lld\n",hcount);
    printf("Total Population Count: %lld\n\n",pcount);
    return SUCCESS;
}

///Function:    count_city
///Description: counts the population of the city
int count_city()
{
    struct state *s;
    struct city *c;
    struct locality *l;
    long long int pcount=0,hcount=0;
    s=head;
    int id;
    printf("\nEnter the city ID to count: ");
    scanf("%d",&id);
    while(s!=NULL)
    {
        c=s->incity;
        while(c!=NULL)
        {
            if(c->cityid==id)
                break;
            c=c->nextcity;
        }
        if(c!=NULL && c->cityid==id)
            break;
        s=s->nextstate;
    }
    if(s==NULL)
    {
        printf("\n---- No Matching cities Found ----\n");
        return FAILURE;
    }
    else
    {
        l=c->inlocality;
        while(l!=NULL)
        {
            pcount+=l->popcount;
            hcount+=l->housecount;
            l=l->nextlocality;
        }
    }
    printf("\nCity Name: %s\n",c->cityname);
    printf("Total House Count: %lld\n",hcount);
    printf("Total Population Count: %lld\n\n",pcount);
    return SUCCESS;
}

///Function:    count_locality
///Description: counts the population of the locality
int count_locality()
{
    struct state *s;
    struct city *c;
    struct locality *l;
    long long int pcount=0,hcount=0;
    s=head;
    int id;
    printf("\nEnter the locality ID to count: ");
    scanf("%d",&id);
    while(s!=NULL)
    {
        c=s->incity;
        while(c!=NULL)
        {
            l=c->inlocality;
            while(l!=NULL)
            {
                if(l->localityid==id)
                    break;
                l=l->nextlocality;
            }
            if(l!=NULL && l->localityid==id)
                break;
            c=c->nextcity;
        }
        if(l!=NULL && l->localityid==id)
                break;
        s=s->nextstate;
    }
    if(s==NULL)
    {
        printf("\n---- No Matching localities Found ----\n");
        return FAILURE;
    }
    else
    {
        printf("\nCity Name: %s\n",l->localityname);
        printf("Total House Count: %d\n",l->housecount);
        printf("Total Population Count: %d\n\n",l->popcount);
        return SUCCESS;
    }
}

///Function: create_graph
///Description: creates a graph for given state.
int create_graph()
{
    FILE *fp;
    fp=fopen("path.txt","r");
    if(fp==NULL)
    {
        printf("------- There is no such file named path.txt -------\n");
        return FAILURE;
    }
    if(file_empty_check("path.txt")!=1)
        return FAILURE;
    char name[15];
    fscanf(fp,"%s",name);
    struct state *s;
    s=head;
    while(s!=NULL)
    {
        if(strcmp(s->statename,name)==0)
            break;
        s=s->nextstate;
    }
    for(int i=0;i<450;i++)
        for(int j=0;j<450;j++)
        {
            if(i==j)
                s->graph[i][j]=0;
            else
                s->graph[i][j]=9999;
        }
    char u[10],v[10];
    float w;
    int u1,v1;
    while(!feof(fp))
    {
        fscanf(fp,"%s%s%f",u,v,&w);
        u1=((u[2]-48)+10*(u[1]-48)-1)*30+((u[4]-48)+10*(u[3]-48)-1);
        v1=((v[2]-48)+10*(v[1]-48)-1)*30+((v[4]-48)+10*(v[3]-48)-1);
        s->graph[u1][v1]=w;
    }
    fclose(fp);
    return SUCCESS;
}

///Function: get_water_level
///Description: get water level of the city
void get_water_level()
{
    int id;
    printf("\nCity for which you want to get water details\nEnter the city ID: ");
    scanf("%d",&id);
    struct state *s;
    struct city *c;
    s=head;
    while(s!=NULL)
    {
        c=s->incity;
        while(c!=NULL)
        {
            if(c->cityid==id)
                break;
            c=c->nextcity;
        }
        if(c!=NULL && c->cityid==id)
            break;
        s=s->nextstate;
    }
    if(c==NULL)
    {
        printf("\n------- No matching cities found -------\n\n");
        return ;
    }
    printf("\nWater level in %s city is %.2f\n\n",c->cityname,c->tankv);
}

///Function: connected
///Description: display all the connected localities
void connected()
{
    char u[10];
    printf("\nEnter the localities ID: ");
    scanf("%s",u);
    int u1=((u[2]-48)+10*(u[1]-48)-1)*30+((u[4]-48)+10*(u[3]-48)-1);
    struct state *s;
    s=head;
    while(s!=NULL)
    {
        if(s->stateid/10000==(u[0]-48))
            break;
    }
    if(s==NULL)
    {
        printf("\n----- No matching data found -----\n");
        return;
    }
    printf("\nList of connected localities:\n");
    for(int i=0;i<450;i++)
    {
        if(s->graph[u1][i]!=0 && s->graph[u1][i]!=INT_MAX)
        {
            int v=(u[0]-48)*10000+(i/30+1)*100+(i%30+1);
            printf("locality Id: %d\n",v);
        }
    }
    printf("\n");
}

///Function: find_pat
///Description: asks for the input pattern
/// and traverses through data
void find_pat()
{
    int flag=0;
    char pat[15];
    printf("\nEnter the pattern: ");
    scanf("%s",pat);
    struct state *s;
    struct city *c;
    struct locality *l;
    s=head;
    printf("\nFound matches: ");
    while(s!=NULL)
    {
        c=s->incity;
        if(pattern(s->statename,pat))
        {
            flag=1;
            printf("\nmatching state name: %s",s->statename);
            for(int i=0;i<23-strlen(s->statename);i++)
                printf(" ");
            printf("state id: %d",s->stateid);
        }
        while(c!=NULL)
        {
            l=c->inlocality;
            if(pattern(c->cityname,pat))
            {
                flag=1;
                printf("\nmatching city name: %s",c->cityname);
                for(int i=0;i<24-strlen(c->cityname);i++)
                    printf(" ");
                printf("city id: %d",c->cityid);
            }
            while(l!=NULL)
            {
                if(pattern(l->localityname,pat))
                {
                    flag=1;
                    printf("\nmatching locality name: %s",l->localityname);
                    for(int i=0;i<20-strlen(l->localityname);i++)
                        printf(" ");
                    printf("locality id: %d",l->localityid);
                }
                l=l->nextlocality;
            }
            c=c->nextcity;
        }
        s=s->nextstate;
    }
    if(flag==0)
        printf("\n\n------- No matches found -------");
    printf("\n\n");
}

///Function: pattern
///Description: check the presence of pattern
int pattern(char txt[15],char pat[15])
{
    int d=30;
    int M = strlen(pat);
    int N = strlen(txt);
    int i, j;
    int p = 0;
    int t = 0;
    int h = 1;
    int q=101;
    for (i = 0; i < M-1; i++)
        h = (h*d)%q;

    for (i = 0; i < M; i++)
    {
        p = (d*p + pat[i])%q;
        t = (d*t + txt[i])%q;
    }
    for (i = 0; i <= N - M; i++)
    {
        if ( p == t )
        {
            for (j = 0; j < M; j++)
            {
                if (txt[i+j] != pat[j])
                    break;
            }
            if (j == M)
                return 1;
        }
        if ( i < N-M )
        {
            t = (d*(t - txt[i]*h) + txt[i+M])%q;
            if (t < 0)
            t = (t + q);
        }
    }
    return 0;
}

///Function: disaster_menu
///Description: prints you the disaster menu
void disaster_menu()
{
    while(1)
    {
        int ch;
        int status;
        printf("\n~~~~~~~~~~ DISASTER MENU ~~~~~~~~~~\n");
        printf("What type of disaster you are having?\n");
        printf("0->Return to main menu\n");
        printf("1-> There is Heavy Rainfall\n");
        printf("2-> Situation of Drought\n");
        printf("Enter your choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 0: printf("(returning to main menu.....)\n\n");
                    return;

            case 1: status=flood();
                        if(status==SUCCESS)
                            printf("\n------ flood management successful ------\n\n");
                        else
                            printf("\n------ flood management failed ------\n\n");
                    return;

            case 2: if(network!=FAILURE)
                    {
                        status=drought();
                        if(status==SUCCESS)
                            printf("\n------ drought management successful ------\n\n");
                        else
                            printf("\n------ drought management failed ------\n\n");
                    }
                    else
                        printf("\n------ water network was not created ------\n\n");
                    return;


            default: printf("\n------ Invalid Choice ------\n");
                     break;
        }
    }
}

///Function: drought
///Description: deals with the drought situation
int create_waternetwork()
{
    FILE *fp;
    fp=fopen("waternetwork.txt","r");
    if(fp==NULL)
    {
        printf("\n------- There is no such file named waternetwork.txt -------\n");
        return FAILURE;
    }
    if(file_empty_check("waternetwork.txt")!=1)
        return FAILURE;
    char name[15];
    fscanf(fp,"%s",name);
    struct state *s;
    s=head;
    while(s!=NULL)
    {
        if(strcmp(s->statename,name)==0)
            break;
        s=s->nextstate;
    }
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
        {
            if(i==j)
                s->waternetwork[i][j]=0;
            else
                s->waternetwork[i][j]=99999;
        }

    char u[10],v[10];
    float w;
    int u1,v1;
    while(!feof(fp))
    {
        fscanf(fp,"%s%s%f",u,v,&w);
        u1=(u[1]-48)*10+(u[2]-48);
        v1=(v[1]-48)*10+(v[2]-48);
        s->waternetwork[u1-1][v1-1]=w;
    }
    fclose(fp);
    return SUCCESS;
}


///Function: all_pair_shortestpath
///Description: computes the shortest path between all pairs of cities
void all_pair_shortestpath(struct state *s)
{
    for(int k=0;k<15;k++)
    {
        for(int i=0;i<15;i++)
        {
            for(int j=0;j<15;j++)
            {
                int sum=s->waternetwork[i][k]+s->waternetwork[k][j];
                if(s->waternetwork[i][j]>sum)
                    s->waternetwork[i][j]=sum;
            }
        }
    }

}

///Function: drought
///Description: deals with the drought situation
int drought()
{
    int ch,id;
    char name[15];
    printf("\nHow do you want to give input?\n");
    printf("1-> City Name\n");
    printf("2-> City ID\n");
    printf("Enter your choice: ");
    scanf("%d",&ch);
    switch(ch)
    {
        case 1: printf("\nEnter the city name: ");
                scanf("%s",name);
                break;

        case 2: printf("\nEnter the city id: ");
                scanf("%d",&id);
                break;

        default: printf("\n----------- Invalid Choice -----------\n");
                 return FAILURE;
    }
    struct state *s;
    struct city *c;
    s=head;
    while(s!=NULL)
    {
        c=s->incity;
        while(c!=NULL)
        {
            if(ch==1 && strcmp(c->cityname,name)==0)
                break;
            else if(ch==2 && c->cityid==id)
                break;
            c=c->nextcity;
        }
        if(c!=NULL && ch==1 && strcmp(c->cityname,name)==0)
                break;
        else if(c!=NULL && ch==2 && c->cityid==id)
                break;
        s=s->nextstate;
    }
    if(c==NULL)
    {
        printf("\n------- No matching cities found ------\n");
        return FAILURE;
    }
    all_pair_shortestpath(s);
    int quan,src,flag=FAILURE;
    src=c->cityid;
    src=src/100-(src/10000)*100;
    printf("\nHow much water do you want?: ");
    scanf("%d",&quan);
    struct city *c1;
    c1=s->incity;
    float watnet[15];
    for(int i=0;i<15;i++)
    {
        if(c1->tankv>=quan)
        {
            flag=SUCCESS;
            if(s->waternetwork[src-1][i]!=0)
                watnet[i]=c1->tankv/s->waternetwork[src-1][i];
            else
                watnet[i]=0;
        }
        else
            watnet[i]=0;
        c1=c1->nextcity;
    }
    if(flag==FAILURE)
    {
        printf("\n--------- No cities have that much of water ---------\n");
        return FAILURE;
    }
    printf("\n");
    int max=0,i;
    for(i=0;i<15;i++)
    {
        if(watnet[i]>watnet[max])
            max=i;
    }
    int dest=(c->cityid/10000)*10000+(max+1)*100;
    c1=s->incity;
    while(c1!=NULL)
    {
        if(c1->cityid==dest)
            break;
        c1=c1->nextcity;
    }
    c1->tankv-=quan;
    c->tankv+=quan;
    printf("Water needs to be transported from city id: %d to city id: %d\n",dest,c->cityid);
    printf("\n-------- Water transported Successfully --------\n");
    create_waternetwork();
    return SUCCESS;
}

///Function: flood
///Description: deals with the flood situation
int flood()
{
    int ch,id;
    char name[15];
    printf("\nHow do you want to give input?\n");
    printf("1-> locality Name\n");
    printf("2-> locality ID\n");
    printf("Enter your choice: ");
    scanf("%d",&ch);
    switch(ch)
    {
        case 1: printf("\nEnter the locality name: ");
                scanf("%s",name);
                break;

        case 2: printf("\nEnter the locality id: ");
                scanf("%d",&id);
                break;

        default: printf("\n----------- Invalid Choice -----------\n");
                 return FAILURE;
    }
    struct state *s;
    struct city *c;
    struct locality *l;
    s=head;
    while(s!=NULL)
    {
        c=s->incity;
        while(c!=NULL)
        {
            l=c->inlocality;
            while(l!=NULL)
            {
                if(ch==1 && strcmp(l->localityname,name)==0)
                    break;
                else if(ch==2 && l->localityid==id)
                    break;
                l=l->nextlocality;
            }
            if(l!=NULL && ch==1 && strcmp(l->localityname,name)==0)
                break;
            else if(l!=NULL && ch==2 && l->localityid==id)
                break;
            c=c->nextcity;
        }
        if(l!=NULL && ch==1 && strcmp(l->localityname,name)==0)
            break;
        else if(l!=NULL && ch==2 && l->localityid==id)
            break;
        s=s->nextstate;
    }
    if(l==NULL)
    {
        printf("\n------- No matching localities found ------\n");
        return FAILURE;
    }

    float rain;
    printf("\nHow much did it rain(in ft): ");
    scanf("%f",&rain);
    if(rain+c->tankv<=CAPACITY)
    {
        c->tankv+=rain;
        printf("\nWater has been stored in city tank");
        printf("\nWater is below safe zone mark: %.2f",c->tankv);
        printf("\nNo flood management is necessary.\n");
        return SUCCESS;
    }
    c->tankv=30;
    printf("\nWater has increased the safe zone mark: %.2f",c->tankv);
    printf("\nThere is a situation of flood");
    printf("\n\nStarting evacuation:\n");
    int start=l->localityid%10000;
    start=(start/100-1)*30+(start%100-1);
    int source=start;
    int visited[450];
    memset(visited,0,450*sizeof(int));
    float dist[450];
    int path[450];

    for(int i=0;i<450;i++)
    {
        dist[i]=s->graph[start][i];
        path[i]=start;
    }
    visited[start]=1;
    float min;
    while(!allvisited(visited))
    {
        min=99999;
        for(int i=0;i<450;i++)
            if(visited[i]==0)
                if(min>dist[i])
                {
                    min=dist[i];
                    start=i;
                }
        visited[start]=1;
        for(int i=0;i<450;i++)
        {
            if(dist[i]>min+s->graph[start][i])
            {
                dist[i]=min+s->graph[start][i];
                path[i]=start;
            }
        }
    }
    int sloc[3],i,dest;
    ///we have assumed three safest localities
    sloc[0]=(309/100-1)*30+(309%100-1);
    sloc[1]=(1121/100-1)*30+(1121%100-1);
    sloc[2]=(914/100-1)*30+(914%100-1);
    printf("There is marked three safest localities:\n");
    printf("Out of which the the most nearest is displayed\n");
    printf("\nPath(stating city ids): \n");
    if(dist[sloc[0]]<=dist[sloc[1]] && dist[sloc[0]]<=dist[sloc[2]])
    {
        dest=sloc[0];
        int act_path[30],k=0;
        for(i=sloc[0];i!=source;i=path[i])
        {
            act_path[k]=i;
            k++;
        }
        act_path[k]=i;
        k++;
        for(i=k-1;i>0;i--)
        {
            int a=act_path[i];
            a=10000+(a/30+1)*100+(a%30+1);
            printf("%d -> ",a);
        }
        int a=act_path[i];
            a=10000+(a/30+1)*100+(a%30+1);
        printf("%d\n\n",a);
        printf("Total Distance: %.2f km\n",dist[dest]);
    }
    else if(dist[sloc[1]]<=dist[sloc[0]] && dist[sloc[1]]<=dist[sloc[1]])
    {
        dest=sloc[1];
        int act_path[30],k=0;
        for(i=sloc[0];i!=source;i=path[i])
        {
            act_path[k]=i;
            k++;
        }
        act_path[k]=i;
        k++;
        for(i=k-1;i>0;i--)
        {
            int a=act_path[i];
            a=10000+(a/30+1)*100+(a%30+1);
            printf("%d -> ",a);
        }
        int a=act_path[i];
            a=10000+(a/30+1)*100+(a%30+1);
        printf("%d\n\n",a);
        printf("Total Distance: %.2f km\n",dist[dest]);
    }
    else if(dist[sloc[2]]<=dist[sloc[0]] && dist[sloc[2]]<=dist[sloc[1]])
    {
        dest=sloc[2];
        int act_path[30],k=0;
        for(i=sloc[0];i!=source;i=path[i])
        {
            act_path[k]=i;
            k++;
        }
        act_path[k]=i;
        k++;
        for(i=k-1;i>0;i--)
        {
            int a=act_path[i];
            a=10000+(a/30+1)*100+(a%30+1);
            printf("%d -> ",a);
        }
        int a=act_path[i];
            a=10000+(a/30+1)*100+(a%30+1);
        printf("%d\n\n",a);
        printf("Total Distance: %.2f km\n",dist[dest]);
    }
    int x=10000+(source/30+1)*100+(source%30+1);
    int y=10000+(dest/30+1)*100+(dest%30+1);
    printf("\nAll the peoples from locality id: %d safely rescued to locality id: %d",x,y);
    printf("\n");
    int temp;
    temp=l->popcount;
    l->popcount=0;
    printf("\n------------- Source Updated Data -------------\n");
    printf("Locality Name: %s\n",l->localityname);
    printf("Locality ID: %d\n",l->localityid);
    printf("State Name:%s\n",s->statename);
    printf("State ID: %d\n",s->stateid);
    printf("City Name:%s\n",c->cityname);
    printf("City ID: %d\n",c->cityid);
    printf("Water Availability: %.2f ft\n",c->tankv);
    printf("Locality house count: %d\n",l->housecount);
    printf("Locality population count: %d\n",l->popcount);
    printf("Locality Altitude: %d\n",l->altitude);
    printf("-----------------------------------------------\n\n");
    s=head;
    while(s!=NULL)
    {
        c=s->incity;
        while(c!=NULL)
        {
            l=c->inlocality;
            while(l!=NULL)
            {
                if(l->localityid==y)
                    break;
                l=l->nextlocality;
            }
            if(l!=NULL && l->localityid==y)
                    break;
            c=c->nextcity;
        }
        if(l!=NULL && l->localityid==y)
            break;
        s=s->nextstate;
    }
    l->popcount+=temp;
    printf("\n----------- Destination Updated Data ----------\n");
    printf("Locality Name: %s\n",l->localityname);
    printf("Locality ID: %d\n",l->localityid);
    printf("State Name:%s\n",s->statename);
    printf("State ID: %d\n",s->stateid);
    printf("City Name:%s\n",c->cityname);
    printf("City ID: %d\n",c->cityid);
    printf("Water Availability: %.2f ft\n",c->tankv);
    printf("Locality house count: %d\n",l->housecount);
    printf("Locality population count: %d\n",l->popcount);
    printf("Locality Altitude: %d\n",l->altitude);
    printf("-----------------------------------------------\n\n");

    printf("blocking the connecting roads.......\n");
    for(int i=0;i<450;i++)
    {
        s->graph[source][i]=9999;
    }
    s->graph[source][source]=0;
    return SUCCESS;
}

///Function: allvisited
///Description: checks whether all nodes are visited or not
int allvisited(int visited[450])
{
    for(int i=0;i<450;i++)
        if(visited[i]==0)
            return 0;
    return 1;
}

///Function: mem_person
///Description: return a node with struct person type
struct mperson *mem_person()
{
    struct mperson *p;
    p=(struct mperson *)malloc(sizeof(struct mperson));
    if(p==NULL)
    {
        printf("\n---- Memory allocation Error ----\n");
        printf("---- Program Terminating ----\n");
        exit(0);
    }
    return p;
}

///Function: missing_person
///Description: adds up the missing person details
void missing_person()
{
    int count;
    printf("\nEnter the no.of person missing: ");
    scanf("%d",&count);
    for(int i=0;i<count;i++)
    {
        printf("\nEnter %d th person details:",i+1);
        struct mperson *p;
        p=mem_person();
        p->right=NULL;
        p->left=NULL;
        printf("\nEnter missing person name: ");
        scanf("%s",p->name);
        printf("Enter person's age: ");
        scanf("%d",&p->age);
        printf("Enter person last seen(days): ");
        scanf("%d",&p->time);
        printf("Enter addition remarks: ");
        scanf("%s",p->remarks);
        insertbst(p);
    }
        printf("\n--------- Updated Successfully ---------\n\n");
}

///Function: insertbst
///Description: inserts into binary search tree
void insertbst(struct mperson *p)
{
    if(root==NULL)
    {
        root=p;
        return;
    }
    struct mperson *cur=root,*parent;
    while(cur!=NULL)
    {
        parent=cur;
        if(p->time<cur->time)
            cur=cur->left;
        else
            cur=cur->right;
    }
    if(p->time>parent->time)
        parent->right=p;
    else
        parent->left=p;
    return;
}

///Function: inorder
///Description: prints the inorder traversal of the tree
void inorder(struct mperson *m)
{
    if(m!=NULL)
    {
        inorder(m->left);
        printf("Missing Person Name: %s\n",m->name);
        printf("Person Age: %d\n",m->age);
        printf("Person Time: %d\n",m->time);
        printf("Remarks: %s\n",m->remarks);
        printf("-----------------------------------\n");
        inorder(m->right);
    }
}
