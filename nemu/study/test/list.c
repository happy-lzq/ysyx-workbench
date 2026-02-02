#include <stdio.h>
#include <stdlib.h>

// ================================单链表结构类型========================= //
    // 定义链表节点数据类型
typedef struct Node
{
    int data;
    struct Node * next;
} Node;

    // 初始化，不带头结点
Node* initlist(){
    return NULL;
}

    // 创建新节点辅助函数
Node* createNode(int data){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

    // 头插法 始终将新节点放在第一位
void insertAthead(Node** head, int data){
    Node* newNode = createNode (data);
    newNode->next = *head;
    *head = newNode;
}

    // 尾插法 找到最后一个节点，让它指向新节点
void insertAttail(Node** head,int data){
    Node* newNode = createNode(data);
    // 防止空链表
    if (*head == NULL){
        *head = newNode;
        return ;
    }
    Node* temp = *head;
    while (temp->next != NULL){
        temp = temp->next;
    }
    temp->next = newNode;
}

    // 指定位置插入（第pos个位置插入，从pos1开始）
void insertAtpos(Node** head, int data,int pos){
    // 处理特殊情况，插入头部
    if (pos == 1){
        Node* newNode = createNode(data);
        newNode->next = *head;
        *head = newNode;
        return ;
    }
    Node* temp = *head;
    // 防止插入尾部
    for (int i = 0; i < pos - 1 && temp != NULL ; i++){
        temp = temp->next;
    }
    // 检查位置是否合法
    if (temp == NULL){
        printf("插入位置无效！\n");
        return;
    } else {
        Node* newNode = createNode(data);
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

    // 链表数据遍历
void printList(Node* head){
    Node* curr = head;
    while (curr != NULL){
        printf("%2d -> ",curr->data);
        curr = curr->next;
    }
    // 最后指向NULL
    printf("NULL\n");
}

    // 指定链表节点的数据释放
void deleteNode(Node** head, int pos){
    if (*head == NULL){
        printf("链表为空，无法删除\n");
        return;
    }
    Node* temp = *head;
    // ① 删除头结点
    if (pos == 1){
        *head = temp->next;
        free (temp);
        return ;
    }
    // ② 中间位置：pos 寻找pos-1前驱节点；
    for (int i = 0; i < pos-1 && temp != NULL; i++){
        temp = temp->next;
    }
    // ③ 检查位置是否合法
    if (temp == NULL || temp->next == NULL){
        printf("delete pos is wuxiao");
        return ;
    }
    // ④执行删除逻辑 target 目标节点，temp 前驱节点
    Node* target = temp->next;
    temp->next = target->next;
    free(target);
}

    // 链表整体数据释放
void freeList(Node** head){
    Node* curr = *head;
    Node* nextNode;
    while (curr != NULL){
        nextNode = curr->next;
        free(curr);
        curr = nextNode;
    }
    *head = NULL;
    printf("List free\n");
}

int main (){
    Node* head = initlist();
    printf("init-List\n");
    printList(head);

    insertAttail(&head,10);
    insertAttail(&head,20);
    insertAttail(&head,30);
    printList(head);

    insertAthead(&head,5);
    printList(head);

    insertAtpos(&head,0,3);
    printList(head);

    deleteNode(&head,2);
    printList(head);

    deleteNode(&head,1);
    printList(head);

    freeList(&head);
    printList(head);
    return 0;

}