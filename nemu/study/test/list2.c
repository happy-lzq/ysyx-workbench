#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct P_information {
    char garde[10];
    char name[10];
    int id_num;
} student;

typedef struct Node {
    student data;
    struct Node *next;
} Node;

/* 封装头指针与元信息的 List 结构 */
typedef struct List {
    Node *head;
    Node *tail;
    size_t size;
} List;

/* 创建一个新节点（复制 student 内容），失败返回 NULL */
static Node *node_new(const student *stu) {
    Node *n = malloc(sizeof *n);
    if (!n) return NULL;
    if (stu) {
        n->data = *stu; /* 结构体整体赋值（包括数组） */
    } else {
        memset(&n->data, 0, sizeof n->data);
    }
    n->next = NULL;
    return n;
}

/* 初始化 List */
void list_init(List *l) {
    if (!l) return;
    l->head = l->tail = NULL;
    l->size = 0;
}

/* 释放整个链表 */
void list_clear(List *l) {
    if (!l) return;
    Node *p = l->head;
    while (p) {
        Node *t = p;
        p = p->next;
        free(t);
    }
    l->head = l->tail = NULL;
    l->size = 0;
}

/* 头插 */
void list_push_front(List *l, const student *s) {
    if (!l) return;
    Node *n = node_new(s);
    if (!n) return;
    n->next = l->head;          // 当前l->head 是指向第一个节点
    l->head = n;
    if (!l->tail) l->tail = n;
    l->size++;
}

/* 尾插（使用 tail 实现 O(1)） */
void list_push_back(List *l, const student *s) {
    if (!l) return;
    Node *n = node_new(s);
    if (!n) return;
    n->next = NULL;
    if (!l->head) {
        l->head = l->tail = n;
    } else {
        l->tail->next = n;
        l->tail = n;
    }
    l->size++;
}

/* 根据 id 查找节点 */
Node *list_find(List *l, int id) {
    if (!l) return NULL;
    for (Node *p = l->head; p; p = p->next) {
        if (p->data.id_num == id) return p;
    }
    return NULL;
}

/* 根据 id 删除第一个匹配节点，返回 1 表示删除成功，0 表示未找到 */
int list_remove_by_id(List *l, int id) {
    if (!l) return 0;
    // 引入二级指针存放一级指针地址，二级指针解引用即可直接修改一级指针指向的地址
    Node **curr = &l->head;
    while (*curr) {
        if ((*curr)->data.id_num == id) {
            Node *tofree = *curr;
            *curr = tofree->next; /* 更新前驱的 next 或 head */
            free(tofree);
            l->size--;
            /* 更新 tail（如果链表变为空或被删的是 tail，需要重新计算 tail） */
            if (!l->head) {
                l->tail = NULL;
            } else {
                Node *p = l->head;
                while (p->next) p = p->next;
                l->tail = p;
            }
            return 1;
        }
        curr = &(*curr)->next;
    }
    return 0;
}

/* 打印链表内容（用于调试） */
void list_print(const List *l) {
    if (!l) return;
    printf("list size=%zu\n", l->size);
    for (Node *p = l->head; p; p = p->next) {
        printf("id=%d, name=%s, garde=%s\n", p->data.id_num, p->data.name, p->data.garde);
    }
}

/* 简单 demo 测试 */
int main(void) {
    List list;
    list_init(&list);

    student s1 = { .garde = "A", .name = "Alice", .id_num = 1 };
    student s2 = { .garde = "B", .name = "Bob", .id_num = 2 };
    student s3 = { .garde = "C", .name = "Carol", .id_num = 3 };

    list_push_back(&list, &s1);
    list_push_back(&list, &s2);
    list_push_front(&list, &s3);

    puts("After insert:");
    list_print(&list);

    printf("find id=2: %s\n", list_find(&list, 2) ? "found" : "not found");

    list_remove_by_id(&list, 2);
    puts("After remove id=2:");
    list_print(&list);

    list_clear(&list);
    puts("After clear:");
    list_print(&list);

    return 0;
}