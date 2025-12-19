// Бинарный поиск в отсортированном массиве целых чисел
int binary_search(int arr[], int size, int target) {
    int left = 0;
    int right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2; // Находим середину
        if (arr[mid] == target) {
            return mid; // Нашли
        } else if (arr[mid] < target) {
            left = mid + 1; // Ищем справа
        } else {
            right = mid - 1; // Ищем слева
        }
    }
    return -1; // Не нашли
}