#include <stdio.h>
#include <stdlib.h>

// 创建结构体数据类型
typedef struct STU{
    char name [20];
    int id_num ;
} STU;

// 创建链表节点数据结构
typedef struct Node{
    STU data;
    struct Node* next;
}Node;

// 链表指针数据结构体
typedef struct List{
    Node* head;
    Node* tail;
    int size;
}List;

// 链表初始化
void list_init( List *l){
    if (!l) return;
    l->head = l->tail = NULL;
    l->size = 0; 
}

// 创建新的节点
Node* createNode(const STU* stu){
    if (!stu) return NULL;
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) return NULL;
    newNode->data = *stu;
    newNode->next = NULL;
    return newNode;
}
// 链表头插
void inserthead(List *l,const STU *stu){
    if (!stu) return;
    Node* newNode = createNode(stu);
    if (!newNode) return;
    newNode->next = l->head;
    l->head = newNode;
    l->size++;
    if (l->size == 1){
        l->tail = newNode;
    }
}

// 链表尾插
void inserttail(List *l, const STU* stu){
    if (!stu) return;
    Node* newNode = createNode(stu);
    if (!newNode) return;
    newNode->next = NULL;
    // 防止仅有一个节点或空链表
    if (l->head == NULL && l->tail == NULL){
        inserthead(l,stu);
    } else {
    // 连接尾节点
    l->tail->next = newNode;
    // 调整尾指针指向新尾节点
    l->tail = newNode;
    l->size++;
    }
}

// 链表指定位置插
void insertpos(List *l, const STU* stu,int pos){
    if (!stu) return;
    if (!l->head || pos <= 1){
        inserthead(l,stu);
        return ;
    } else if (pos > l->size){
        inserttail(l,stu);
        return ;
    } else{
    // 寻找前驱节点
        Node* curr = l->head;
        for (int i = 1; i < pos-1 && curr !=NULL ; i++){
            curr = curr->next;
        }
        Node* newNode = createNode(stu);
        if (!newNode) return;
        newNode->next = curr->next;
        curr->next = newNode;
        l->size++;
        // 如果尾插更新tail
        if (newNode->next == NULL){
            l->tail = newNode;
        }
    }
}

// 链表遍历
void printList(List *l){
    if(!l) return ;
    Node* curr = l->head;
    while (curr){
        printf("%s: %d —> ",curr->data.name,curr->data.id_num);
        curr=curr->next;
    }
    printf("NULL\n");
}
// 链表指定位置释放（删除）
void deleteList(List *l,int pos){
    if(!l) return;
    // 链表节点数小于指定位置、空链表
    if (l->size < pos || l->size ==0 ){
        printf("删除指定位置：%d , 已超出节点范围!\n",pos);
        return ;
    }
    // 链表节点数大于等于指定位置
    Node* curr = l->head;
    if (l->size >= pos){
        if (pos <= 1){
            // 链表数和指定位置都为1，删除之后成空链表
            if (l->size == 1){
                l->head = NULL;
                l->tail = NULL;
                free(curr);
                l->size = 0;
                return ;
            // 链表数>=2，指定删除头节点1
            } else{
                l->head = curr->next;
                free (curr);
                l->size--;
                return;
            }
            // 链表节点数和指定位置 >=2, 
        } else {
            for (int i = 1; i < pos-1 && curr != NULL; i++){
                curr = curr->next;
            } 
            Node* target = curr->next;
            curr->next = target->next;
            // 刚好删除尾节点，调整尾节点为前驱节点
            if (pos == l->size ){
                l->tail = curr;
            }
            free(target);
            l->size--;
            return ;
        }
    }
}

// 链表整体释放
void freeList(List *l){
    if(!l) return;
    Node* curr = l->head;
    Node* nextnode ={0};
    while (curr){
        nextnode = curr->next;
        free(curr);
        curr = nextnode;
        l->size--;
    }
    if (curr == NULL && l->size ==0){
        printf("List was freed!\n");
    }
    l->head = l->tail = NULL;
}

STU stu1 = {.name = "别子庆", .id_num = 1001};
STU stu2 = {.name = "余伟杰", .id_num = 1002};
STU stu3 = {.name = "李一庚", .id_num = 1003};
STU stu4 = {.name = "周易书", .id_num = 1004};

int main(){
    List l;
    list_init(&l);
    inserthead(&l,&stu1);
    inserttail(&l,&stu2);
    insertpos(&l,&stu3,2);
    printList(&l);

    deleteList(&l,1);
    printList(&l);

    deleteList(&l,3);
    insertpos(&l,&stu4,3);
    printList(&l);

    freeList(&l);
    printList(&l);

    return 0;
}