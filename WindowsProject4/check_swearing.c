#include <string.h>
#include <stdlib.h>

// �����໰����
static const char* swear_words[] = {
"װ��", "������","�Բ�","�����", "�����", "ɵ��", "ܳ","�H", "���յ�", "����", "����", "����", "����", "ɵ��","���", "����", "�׳�", "�Բ�", "����","������", "ȥ����", "������", "����", "���˸�����","�ϱ�", "ɧ��", "��ɵ��", "����", "���", "װ�Ʒ�","ɵ��","㶱�", "����", "����", "��ܳ", "��ܳ", "�ղ�","cnm", "nmb", "sb", "nc", "djb", "nt",
"fuck", "shit", "asshole", "bitch", "dick", "cock", "pussy","cunt", "motherfucker", "son of a bitch", "whore", "slut","douchebag", "prick", "bastard", "twat", "arsehole", "nigger","faggot", "retard", "stupid", "idiot", "moron"
};
static const int WORD_COUNT = 7;

// KMP�㷨��next�������
static void compute_next(const char* pattern, int* next, int len) {
    int j = 0;
    next[0] = 0;

    for (int i = 1; i < len; i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = next[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        next[i] = j;
    }
}

// KMP�ַ���ƥ��
static int kmp_search(const char* text, const char* pattern) {
    int n = strlen(text);
    int m = strlen(pattern);

    if (m == 0) return 0;

    int* next = (int*)malloc(m * sizeof(int));
    if (!next) return -1;

    compute_next(pattern, next, m);

    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && text[i] != pattern[j]) {
            j = next[j - 1];
        }
        if (text[i] == pattern[j]) {
            j++;
        }
        if (j == m) {
            free(next);
            return i - j + 1; // �ҵ�ƥ��
        }
    }

    free(next);
    return -1; // δ�ҵ�ƥ��
}

int detect_swear_words(const char* text) {
    if (!text) return 0;

    for (int i = 0; i < WORD_COUNT; i++) {
        if (kmp_search(text, swear_words[i]) >= 0) {
            return 1; // �����໰
        }
    }

    return 0; // �������໰
}