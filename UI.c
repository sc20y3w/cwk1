#define _CRT_SECURE_NO_WARNINGS
#define MAXLEN 10240

#include "UI.h"
#include "book_management.h"
#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BookList *books;
Book *p2;
Book book;
int choice;
struct users us;
struct users *users = &us;

char current_user[100];
char root_name[100] = "librarian";   // librarian
char root_passwd[100] = "librarian"; // librarian

char* path1 = "../users.txt";
char* path2 = "../books.txt";

static char *ask_question(const char *question) {
    printf("%s",question);

    const int size_step = 2;
    char *answer = malloc(size_step);
    answer[0] = 0;

    return answer;
}

void read_users() {
    FILE *fp;
    // read users.txt
    fp = fopen(path1, "r");
    if (fp == NULL) {
        printf("File (%s) open failed, end program",path1);
        exit(0);
    }
    users->top = -1;
    int i;
    for (i = 0; !feof(fp); i++) {
        fscanf(fp, "%s %s %d", users->us[i].account, users->us[i].passwd, &users->us[i].bn);
        for (int j = 0; j < users->us[i].bn; j++) {
            fscanf(fp, "%d", &users->us[i].bk[j]);
        }
    }
    users->top = i - 1;
    // show users.txt
    //for (int i = 0; i <= users->top; i++)
    //{
    //    printf("%s %d\n", users->us[i].account, users->us[i].bn);
    //    for (int j = 0; j < users->us[i].bn; j++)
    //    {
    //        printf("%d ", users->us[i].bk[j]);
    //    }
    //    putchar('\n');
    //}
    fclose(fp);
}

void read_books() {
    FILE *fp;
    // read books.txt
    struct _BookList *head;
    head = init_book_list();
    fp = fopen(path2, "r");
    if (fp == NULL) {
        printf("File (%s) open failed, end program", path2);
        exit(0);
    }
    Book *book_p, *pre = head->list;
    char c;
    char tmp[100];
    memset(tmp, '\0', 100);
    int j = 0, k = 1;
    int count = 0;

    char buffer[100]; // buffer to save one-line data
    while ((fgets(buffer,255,fp))!=NULL) {
        int id,year,copies;
        char* title = (char*) malloc(sizeof(char)*100);
        char* authors = (char*) malloc(sizeof(char)*100);

        sscanf(buffer,"%u %s %s %u %u",&id,title,authors,&year,&copies); // split string
        book_p = (Book *) malloc(sizeof(Book));
        book_p->next = NULL;
        book_p->id = id;
        book_p->title = title;
        book_p->authors = authors;
        book_p->year = year;
        book_p->copies = copies;

        if(pre == NULL){
            head->list = book_p;
            pre = book_p;
        }else{
            pre->next = book_p;
            pre = book_p;
        }
        count++;
    }
    books = head;
    books->length = count;

//    display_all_books();

    fclose(fp);
}

void read_file() {
    read_users();
    read_books();
}

void write_file() {
    FILE *f;
    // write users to users.txt
    f = fopen(path1, "w+");
    if(f==NULL){
        printf("File (%s) open failed, end program", path1);
        exit(0);
    }
    int i;
    for (i = 0; i < users->top; i++) {
        fprintf(f, "%s %s %d\n", users->us[i].account, users->us[i].passwd, users->us[i].bn);
        for (int j = 0; j < users->us[i].bn; j++) {
            fprintf(f, "%d\n", users->us[i].bk[j]);
        }
    }
    fprintf(f, "%s %s %d\n", users->us[i].account, users->us[i].passwd, users->us[i].bn);
    int j;
    for (j = 0; j < users->us[i].bn - 1; j++) {
        fprintf(f, "%d\n", users->us[i].bk[j]);
    }
    fprintf(f, "%d", users->us[i].bk[j]);
    fclose(f);
    // write books to books.txt
    f = fopen(path2, "w+");
    if(f == NULL){
        printf("File (%s) open failed, end program", path2);
    }
    struct _BookList *head;
    head = init_book_list();
    Book *p = head->list;
    for (int i = 0; i < books->length; i++) {
        fprintf(f, "%d|%s|%s|%d|%d\n", p->id, p->title, p->authors, p->year, p->copies);
        p = p->next;
    }
    fprintf(f, "%d|%s|%s|%d|%d", p->id, p->title, p->authors, p->year, p->copies);
    fclose(f);
}

BookList find_book_by_title () {
    Book *p = books->list;
    char title[100];
    printf("Please enter title: ");
    gets(title);
    printf("title= %s\n",title);
    printf("ID\tTitle\t\t\t\tAuthor\t\t\t\tyear\t\t\t\tcopies\n");
    while (p->next != NULL) {
//        p = p->next;
        if (strcmp(p->title, title) == 0) {
            printf("%d \t%s \t%s \t%d \t%d\n", p->id, p->title, p->authors, p->year, p->copies);
        }
        p = p->next;
    }
    return *books;
}

BookList find_book_by_author () {
    Book *book_p = books->list;
    char author[100];
    printf("Please enter author: ");
    gets(author);
    printf("ID\tTitle\tAuthor\tyear\tcopies\n");
    while (book_p->next != NULL) {
        if (strcmp(book_p->authors, author) == 0) {
            printf("%d \t%s \t%s \t%d \t%d\n", book_p->id, book_p->title, book_p->authors, book_p->year, book_p->copies);
        }
        book_p = book_p->next;
    }
}

BookList find_book_by_year () {
    Book *book_p = books->list;
    printf("Please enter year: ");
    unsigned int year;
    scanf("%u", &year);
    getchar();
    printf("ID\tTitle\tAuthor\tyear\tcopies\n");
    while (book_p->next != NULL) {
        book_p = book_p->next;
        if (book_p != NULL && book_p->year == year) {
            printf("%d \t%s \t%s \t%d \t%d\n", book_p->id, book_p->title, book_p->authors, book_p->year, book_p->copies);
        }
    }
}

void search_all_books() {
    while (1) {
        printf("Please choose an option:\n");
        printf("1) Find books by title\n");
        printf("2) Find books by author\n");
        printf("3) Find books by year\n");
        printf("4) Return to previous menu\n Option: ");
        scanf("%d", &choice);
        getchar();
        Book *p = books->list;
        if (choice == 1) {
            find_book_by_title();
        } else if (choice == 2) {
            find_book_by_author();
        } else if (choice == 3) {
            find_book_by_year();
        } else if (choice == 4) {
            break;
        } else {
        }
    }
}


static void register_account() {
    char username[100];
    char passwd[100];

    printf("Please enter a username: ");
    scanf("%s", username);
    printf("Please enter a password: ");
    scanf("%s", passwd);
    int i;
    for (i = 0; i <= users->top; i++) {
        if (strcmp(username, users->us[i].account) == 0 || strcmp(username, root_name) == 0) {
            printf("Sorry,registration unsuccessful,the username you entered already exists.\n");
            break;
        }
    }
    if (i > users->top) {
        users->top++;
        strcpy(users->us[users->top].account, username);
        strcpy(users->us[users->top].passwd, passwd);
        users->us[users->top].bn = 0;
        printf("successful\n");
    }
}

static void display_all_books() {
//    struct _BookList *head;
//    head = init_book_list();
    Book *p = books->list;
    printf("ID\tTitle\tAuthor\tyear\tcopies\n");
//    FILE *fp = fopen(path2, "r");
//    if (fp == NULL) {
//        printf("Fail to display.");
//        exit(0);
//    }
    while (p != NULL) {
        printf("%d \t%s \t%s \t%d \t%d\n", p->id, p->title, p->authors, p->year, p->copies);
        p = p->next;
    }
//    fclose(fp);
    return;
}

int add_book(Book book) {
    Book *p, *pre = books->list;
    p = (Book *) malloc(sizeof(Book));
    char tmp[100];
    printf("Enter the title of the book you wish to add: ");
    memset(tmp, '\0', 100);
    gets(tmp);
    p->title = (char *) malloc(sizeof(strlen(tmp)));
    strcpy(p->title, tmp);
    printf("Enter the author of the book you wish to add: ");
    memset(tmp, '\0', 100);
    gets(tmp);
    p->authors = (char *) malloc(sizeof(strlen(tmp)));
    strcpy(p->authors, tmp);
    printf("Enter the year that the book you wish to add was release: ");
    scanf("%d", &p->year);
    getchar();
    printf("Enter the number of copies of the book that you wish to add: ");
    scanf("%d", &p->copies);
    getchar();
    books->length++;

    p->id = books->length + 1;

    while (pre->next != NULL) {
        pre = pre->next;
    }
    pre->next = p;
    p->next = NULL;
    printf("add successfully\n");
    return 0;
}

int remove_book(Book book){
    int id;
    printf("Please enter the number of the book  to be removed: ");
    scanf("%d", &id);
    getchar();
    book.id = id;
    struct _BookList *head;
    head = init_book_list();
    Book *p = head->list, *pre;
    while (p != NULL) {
        if (p->id == book.id) {
            pre->next = p->next;
            free(p);
            // p = pre->next;
            books->length--;
            break;
        } else {
            pre = p;
            p = p->next;
        }
    }
    return 0;
}

void root_manage() {
    int choice;
    do {
        printf("\n(logged in as : %s)", root_name);
        printf("\nPlease choose an option:\n");
        printf("1) Add a book\n");
        printf("2) Remove a book\n");
        printf("3) Search for books\n");
        printf("4) Display all books\n");
        printf("5) Log out\n Option: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                add_book(book);
                break;
            case 2:
                remove_book(book);
                break;
            case 3:
                search_all_books();
                break;
            case 4:
                display_all_books();
                break;
            case 5:
                printf("goodbye");
                break;
            default:
                getchar();
                printf("Sorry, the option you entered was invalid, please try again.\n");
        }
    } while (choice != 5);
}

void borrow_book() {
    int id;
    printf("Blew is the list of Books you can borrow:\n");
    display_all_books();
    printf("Enter the ID number of the book you wish to borrow: ");
    scanf("%d", &id);
    getchar();
    int i;
    for (i = 0; i <= users->top; i++) {
        if (strcmp(current_user, users->us[i].account) == 0 && id > 0) {
            users->us[i].bk[users->us[i].bn] = id;
            users->us[i].bn++;
            break;
        }
    }
    if (i > users->top) {
        printf("Borrowed book not successfully\n");
    } else {
        printf("Borrowed book successfully\n");
    }
}
void return_book() {
    struct _BookList *head;
    head = init_book_list();
    int id;
    printf("Blew is the list of Books you are currently borrowing:\n");
    printf("ID\tTitle\tAuthor\tyear\tcopies\n");

    for (int i = 0; i <= users->top; i++) {
        if (strcmp(users->us[i].account, current_user) == 0) {
            for (int j = 0; j < users->us[i].bn; j++) {
                Book *p = head->list;
                while (p != NULL) {
                    if (p->id == users->us[i].bk[j]) {
                        printf("%d \t%s \t%s \t%d \t%d\n", p->id, p->title, p->authors, p->year, p->copies);
                    }
                    p = p->next;
                }
            }
        }
    }

    printf("Enter the ID number of the book you wish to return: ");
    scanf("%d", &id);
    getchar();
    int i, count = 0;
    for (i = 0; i <= users->top; i++) {
        if (strcmp(current_user, users->us[i].account) == 0 && id < books->length + 1) {
            for (int j = 0; j < users->us[i].bn; j++) {
                if (users->us[i].bk[j] == id) {
                    for (int k = j; k < users->us[i].bn - 1; k++) {
                        users->us[i].bk[k] = users->us[i].bk[k + 1];
                    }
                    break;
                }
                count++;
            }
            users->us[i].bn--;
            break;
        }
    }
    if (count > users->us[i].bn) {
        printf("Returned book not successfully\n");
    } else {
        printf("Returned book successfully\n");
    }
}

void user_manage() {
    int choice;
    do {
        printf("\n(logged in as : %s)", current_user);
        printf("\nPlease choose an option:\n");
        printf("1) Borrow a book\n");
        printf("2) Return a book\n");
        printf("3) Search for books\n");
        printf("4) Display all books\n");
        printf("5) Log out\n Option: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                borrow_book();
                break;
            case 2:
                return_book();
                break;
            case 3:
                search_all_books();
                break;
            case 4:
                display_all_books();
                break;
            case 5:
                printf("goodbye");
                break;
            default:
                getchar();
                printf("Sorry, the option you entered was invalid, please try again.\n");
        }
    } while (choice != 5);
}

void login_account() {
    char username[100];
    char passwd[100];
    printf("Please enter a username: ");
    scanf("%s", username);
    printf("Please enter a password: ");
    scanf("%s", passwd);
    strcpy(current_user, username);
    if (strcmp(username, root_name) == 0) {
        if (strcmp(passwd, root_passwd) == 0) {
            root_manage();
            return;
        }
    } else {
        for (int i = 0; i <= users->top; i++) {
            if (strcmp(username, users->us[i].account) == 0 && strcmp(passwd, users->us[i].passwd) == 0) {
                user_manage();
                return;
            }
        }
    }
    printf("login error\n");
}

static void main_menu() {

    int choice = 5;

    do{
        char * answer = ask_question("\nPlease choose an option:\n1) Register an account\n2) Login\n3) Search for books\n4) Display all books\n5) Quit\n Option: ");
        choice = atoi(answer);
        scanf("%d", &choice);
        free(answer);

        switch (choice) {
            case 1:
                register_account();
                break;
            case 2:
                login_account();
                break;
            case 3:
                search_all_books();
                break;
            case 4:
                display_all_books();
                break;
            case 5:
                printf("goodbye");
                break;
            default:
                getchar();
                printf("Sorry, the option you entered was invalid, please try again.\n");
        }
    } while (choice != 5);

    return;
}

void run_interface() {
    read_users();
    read_books();
    main_menu();
    write_file();
    return;
}
