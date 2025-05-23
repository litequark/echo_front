#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// �����໰����
static const char* swear_words[] = {
    // �����໰
    "װ��", "������", "�Բ�", "�����", "�����", "ɵ��", "ܳ",
    "�H", "���յ�", "����", "����", "����", "����", "ɵ��",
    "���", "����", "�׳�", "�Բ�", "����", "������", "ȥ����",
    "������", "����", "���˸�����", "�ϱ�", "ɧ��", "��ɵ��",
    "����", "���", "װ�Ʒ�", "ɵ��", "㶱�", "����", "����",
    "��ܳ", "��ܳ", "�ղ�", "cnm", "nmb", "sb", "nc", "djb", "nt",

    // Ӣ���໰�������Ƚ����������ƥ��Ч�ʣ�
    "motherfucker", "son of a bitch", "asshole", "douchebag",
    "sonofabitch", "fuck", "shit", "bitch", "dick", "cock",
    "pussy", "cunt", "whore", "slut", "prick", "bastard",
    "twat", "arsehole", "nigger", "faggot", "retard",
    "stupid", "idiot", "moron"
};

// �Զ��������鳤��
static const int WORD_COUNT = sizeof(swear_words) / sizeof(swear_words[0]);

// ת��ΪСд������Ӣ���໰�����ִ�Сдƥ�䣩
static char* to_lowercase(char* str) {
    if (!str) return NULL;
    char* p = str;
    while (*p) {
        *p = tolower((unsigned char)*p);
        p++;
    }
    return str;
}

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

// KMP�ַ���ƥ�䣨��ǿ�棬֧�ִ�Сд������ƥ�䣩
static int kmp_search(const char* text, const char* pattern, int case_sensitive) {
    int n = strlen(text);
    int m = strlen(pattern);

    if (m == 0) return 0;

    // ����ģʽ���ı��ظ������ڴ�Сдת��
    char* pattern_local = strdup(pattern);
    if (!pattern_local) return -1;

    // ��������ִ�Сд��ת��ΪСд
    if (!case_sensitive) {
        to_lowercase(pattern_local);

        // �����ı��ı��ظ������ڴ�Сдת��
        char* text_local = strdup(text);
        if (text_local) {
            to_lowercase(text_local);
            text = text_local; // ʹ��ת������ı�����ƥ��
        }
    }

    int* next = (int*)malloc(m * sizeof(int));
    if (!next) {
        free(pattern_local);
        return -1;
    }

    compute_next(pattern_local, next, m);

    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && text[i] != pattern_local[j]) {
            j = next[j - 1];
        }
        if (text[i] == pattern_local[j]) {
            j++;
        }
        if (j == m) {
            free(next);
            free(pattern_local);
            if (!case_sensitive) free((void*)text);
            return i - j + 1; // �ҵ�ƥ��
        }
    }

    free(next);
    free(pattern_local);
    if (!case_sensitive) free((void*)text);
    return -1; // δ�ҵ�ƥ��
}

// ����ı����Ƿ�����໰
int detect_swear_words(const char* text) {
    if (!text) return 0;

    // ���������໰����ƥ��
    for (int i = 0; i < WORD_COUNT; i++) {
        // �ж��Ƿ�ΪӢ���໰����ͨ�����ַ��жϣ�
        int is_english = (unsigned char)swear_words[i][0] < 128;

        // Ӣ���໰�����ִ�Сд�������໰����
        if (kmp_search(text, swear_words[i], !is_english) >= 0) {
            return 1; // �����໰
        }
    }

    return 0; // �������໰
}