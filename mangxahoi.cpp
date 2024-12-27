#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_FRIENDS 10
#define MAX_HOBBIES 3
#define DATA_FILE "users_data.txt"  // File luu tru thong tin nguoi dung

// Cau truc du lieu cho nguoi dung
typedef struct {
    char studentID[20];  // Ma so sinh vien
    char fullName[100];   // Ten day du
    char hobbies[MAX_HOBBIES][100]; // So thich
    int numFriends; 
    char posts[20][500];  // Luu các bài vi?t
    int numPosts;
} User;

User users[MAX_USERS];
int graph[MAX_USERS][MAX_USERS];
int numUsers = 0;

// Bien kiem tra nguoi dung da dang nhap hay chua
User* loggedInUser = NULL;

// Ham luu du lieu nguoi dung vao file
void saveData() {
    FILE *file = fopen(DATA_FILE, "w");
    if (!file) {
        printf("Du lieu chua duoc luu vao file.\n");
        return;
    }

    for (int i = 0; i < numUsers; i++) {
        fprintf(file, "%s\n", users[i].studentID);
        fprintf(file, "%s\n", users[i].fullName);
         fprintf(file, "%d\n", users[i].numPosts); // S? lu?ng bài vi?t c?a ngu?i dùng
    
    // Luu bài vi?t
        for (int j = 0; j < users[i].numPosts; j++) {
            fprintf(file, "%s\n", users[i].posts[j]);
        }
    }
        // Luu ma tr?n d? th?
    for (int i = 0; i < numUsers; i++) {
        for (int j = 0; j < numUsers; j++) {
            fprintf(file, "%d ", graph[i][j]);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Du lieu da duoc luu vao file.\n");
}

// Ham tai du lieu nguoi dung tu file
void loadData() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) {
        printf("Du lieu chua duoc tai tu file.\n");
        return;
    }

    while (fscanf(file, "%s\n", users[numUsers].studentID) != EOF) {
        fgets(users[numUsers].fullName, sizeof(users[numUsers].fullName), file);
        users[numUsers].fullName[strcspn(users[numUsers].fullName, "\n")] = '\0';  // Loai bo ky tu newline
        users[numUsers].numFriends = 0;  // M?c d?nh không có b?n bè
        users[numUsers].numPosts = 0;  // M?c d?nh không có bài vi?t
        int numPosts;
        fscanf(file, "%d\n", &numPosts);
        users[numUsers].numPosts = numPosts;

        // T?i bài vi?t
        for (int i = 0; i < numPosts; i++) {
            fgets(users[numUsers].posts[i], sizeof(users[numUsers].posts[i]), file);
            users[numUsers].posts[i][strcspn(users[numUsers].posts[i], "\n")] = '\0';  // Lo?i b? ký t? newline
        }

        numUsers++;
    }
     // T?i ma tr?n d? th?
    for (int i = 0; i < numUsers; i++) {
        for (int j = 0; j < numUsers; j++) {
            fscanf(file, "%d", &graph[i][j]);
        }
    }


    fclose(file);
    printf("Du lieu da duoc tai tu file.\n");
}

// Ham dang ky nguoi dung moi
int registerUser(char studentID[]) {
    if (numUsers >= MAX_USERS) {
        printf("Danh sach nguoi dung da day.\n");
        return 0;
    }

    // Kiem tra xem ma so sinh vien da ton tai chua
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].studentID, studentID) == 0) {
            printf("Ma so sinh vien da ton tai\n");
            return 0;
        }
    }

    // Them nguoi dung moi
    strcpy(users[numUsers].studentID, studentID);
    users[numUsers].numFriends = 0;
    users[numUsers].numPosts = 0;

    // Yeu cau nhap thong tin ca nhan
    printf("Dang ky thanh cong! Bay gio, vui long nhap thong tin ca nhan cua ban:\n");

    printf("Nhap ten day du: ");
    fgets(users[numUsers].fullName, sizeof(users[numUsers].fullName), stdin);
    users[numUsers].fullName[strcspn(users[numUsers].fullName, "\n")] = '\0'; // Loai bo ky tu newline

    // Nhap so luong so thich va kiem tra tinh hop le
    int numHobbies;
    do {
        printf("Nhap so luong so thich cua ban (Toi da %d so thich): ", MAX_HOBBIES);
        scanf("%d", &numHobbies);
        getchar(); // Doc dau Enter thua

        if (numHobbies < 0 || numHobbies > MAX_HOBBIES) {
            printf("So luong so thich khong hop le. Vui long nhap lai tu 0 den %d.\n", MAX_HOBBIES);
        } else {
            break; // Neu so luong hop le, thoat khoi vong lap
        }
    } while (1);

    // Nhap so thich
    for (int i = 0; i < numHobbies; i++) {
        printf("Nhap so thich %d: ", i + 1);
        fgets(users[numUsers].hobbies[i], sizeof(users[numUsers].hobbies[i]), stdin);
        users[numUsers].hobbies[i][strcspn(users[numUsers].hobbies[i], "\n")] = '\0'; // Loai bo ky tu newline
    }

    numUsers++;
    printf("Thong tin ca nhan da duoc luu!\n");
    printf("--------------------------------------\n");
    
    return 1;
}

// Ham dang nhap nguoi dung
User* login(char studentID[]) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].studentID, studentID) == 0) {
            return &users[i];  // Tra ve con tro den nguoi dung da dang nhap
        }
    }
    printf("Ma so sinh vien khong ton tai.\n");
    return NULL;  // Tra ve NULL neu khong tim thay nguoi dung
}

// Ham them ban be
void addFriend(User* currentUser, char friendStudentID[]) {
    int friendIndex = -1;
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].studentID, friendStudentID) == 0) {
            friendIndex = i;
            break;
        }
    }

    if (friendIndex == -1) {
        printf("Khong tim thay ban voi ma sinh vien nay.\n");
        return;
    }

    // Ki?m tra n?u dã là b?n bè
    if (graph[friendIndex][currentUser - users] == 1) {
        printf("Nguoi dung da la ban be.\n");
        return;
    }

    // Thêm b?n bè vào ma tr?n d? th?
    graph[friendIndex][currentUser - users] = 1;
    graph[currentUser - users][friendIndex] = 1;

    printf("Them ban thanh cong.\n");
}
// Ham goi y ket ban theo so thich
void suggestFriendsByHobbies(User* currentUser) {
    if (numUsers <= 1) {
        printf("Khong co nguoi dung nao de goi y.\n");
        return;
    }

    printf("Goi y ket ban dua tren so thich:\n");

    // Duyet qua cac nguoi dung khac
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].studentID, currentUser->studentID) == 0) {
            continue; // Bo qua chinh nguoi dung hien tai
        }
        // Ki?m tra s? thích chung
        int commonHobbies = 0;
        for (int j = 0; j < MAX_HOBBIES && currentUser->hobbies[j][0] != '\0'; j++) {
            for (int k = 0; k < MAX_HOBBIES && users[i].hobbies[k][0] != '\0'; k++) {
                if (strcmp(currentUser->hobbies[j], users[i].hobbies[k]) == 0) {
                    commonHobbies++;
                    break;
                }
            }
        }
        
        // Neu co it nhat mot so thich chung, goi y ket ban
        if (commonHobbies > 0) {
            printf("%s (%d so thich chung)\n", users[i].fullName, commonHobbies);
            printf("%s\n",users[i].studentID);
        }
    }
}
// Hàm hi?n th? danh sách b?n bè c?a ngu?i dùng
void viewFriends(User* currentUser) {
    int hasFriends = 0;
    printf("Danh sach ban be:\n");

    for (int i = 0; i < numUsers; i++) {
        if (graph[currentUser - users][i] == 1) {
            printf("%s\n", users[i].fullName);
            hasFriends = 1;
        }
    }

    if (!hasFriends) {
        printf("Ban khong co ban be nao\n");
    }
}

// Hàm hi?n th? ngu?i có nhi?u b?n nh?t
void showMostFriends() {
    if (numUsers == 0) {
        printf("Chua co nguoi dung trong he thong \n");
        return;
    }

    int maxFriends = -1;
    int indexOfMax = -1;

    for (int i = 0; i < numUsers; i++) {
        int friendCount = 0;
        for (int j = 0; j < numUsers; j++) {
            if (graph[i][j] == 1) {
                friendCount++;
            }
        }

        if (friendCount > maxFriends) {
            maxFriends = friendCount;
            indexOfMax = i;
        }
    }

    printf("Nguoi co nhieu ban nhat la: %s voi %d ban.\n", users[indexOfMax].fullName, maxFriends);
}

// Hàm ngu?i dùng dang bài vi?t m?i
void createPost(User* currentUser) {
    if (currentUser->numPosts >= 20) {
        printf("Ban da dang qua bai viet toi da\n");
        return;
    }
    char i;
    do{
        printf("Nhap noi dung bai viet: ");
        fgets(currentUser->posts[currentUser->numPosts], 500, stdin);
        currentUser->posts[currentUser->numPosts][strcspn(currentUser->posts[currentUser->numPosts], "\n")] = '\0'; // Lo?i b? ký t? newline
        currentUser->numPosts++;
        printf("Bai viet cua ban da duoc dang!\n");
        printf("Tiep tuc dang bai ? (y/n): ");
        scanf("%c",&i);
        getchar();
    }while(i!='n');
}

// Hàm xem các bài vi?t c?a ngu?i dùng
void viewPosts(User* currentUser) {
    if (currentUser->numPosts == 0) {
        printf("Ban chua co bai viet nao.\n");
    } else {
        printf("\n===============================\n");
        printf("%s\n", currentUser->fullName);
        printf("ID: %s\n", currentUser->studentID);
        printf("===============================\n");
        printf("Danh sach bai viet cua ban:\n");
        for (int i = 0; i < currentUser->numPosts; i++) {
            printf("%d. %s\n", i + 1, currentUser->posts[i]);
        }
        printf("\n");
    }
}
//Hàm xem bài vi?t c?a b?n
void viewFriendsPosts(User* currentUser) {
   int hasFriends = 0;

    // Hi?n th? danh sách b?n bè
    printf("Danh sach ban be:\n");
    for (int i = 0; i < numUsers; i++) {
        if (graph[currentUser - users][i] == 1) {  // Ki?m tra xem có ph?i b?n bè không
            hasFriends = 1;
            printf("%s | %s\n", users[i].studentID, users[i].fullName); // Hi?n th? tên và ID b?n bè
        }
    }

    if (!hasFriends) {
        printf("Ban khong co ban be nao.\n");
        return;
    }

    printf("Nhap ID cua ban de xem bai viet: ");
    char friendID[50];
    scanf("%s", friendID);
    getchar(); // Ð?c ký t? Enter du th?a

    // Tìm ki?m b?n bè theo ID
    int friendIndex = -1;
    for (int i = 0; i < numUsers; i++) {
        if (graph[currentUser - users][i] == 1 && strcmp(users[i].studentID, friendID) == 0) {
            friendIndex = i;
            break;
        }
    }

    if (friendIndex == -1) {
        printf("Nguoi dung co ID %s khong phai la ban cua ban.\n", friendID);
        return;
    }


    // Hi?n th? các bài vi?t c?a b?n
    if (users[friendIndex].numPosts == 0) {
        printf("Ban cua ban chua co bai viet nao.\n");
    } else {
        printf("\n======================================\n");
        printf("%s\n", users[friendIndex].fullName);
        printf("ID: %s\n", users[friendIndex].studentID);
        printf("======================================\n");
        printf("Danh sach bai viet:\n");
        for (int i = 0; i < users[friendIndex].numPosts; i++) {
            printf("%d. %s\n", i + 1, users[friendIndex].posts[i]);
        }
        printf("\n");
        
    }
}
void taodlmau() {
    // D? li?u ngu?i dùng 1
    strcpy(users[numUsers].studentID, "20234546");
    strcpy(users[numUsers].fullName, "Nguyen Thi Lan");
    users[numUsers].numPosts = 2;
    users[numUsers].numFriends = 2;
    strcpy(users[numUsers].posts[0], "Troi dep nen di dao trong cong vien");
    strcpy(users[numUsers].posts[1], "Happy Birthday to me");
    strcpy(users[numUsers].hobbies[0], "Doc sach");
    strcpy(users[numUsers].hobbies[1], "Chay bo");
    numUsers++;

    // D? li?u ngu?i dùng 2
    strcpy(users[numUsers].studentID, "20230002");
    strcpy(users[numUsers].fullName, "Tran Minh Hoang");
    users[numUsers].numPosts = 1;
    strcpy(users[numUsers].posts[0], "Moi mua laptop");
    strcpy(users[numUsers].hobbies[0], "Choi game");
    numUsers++;

    // D? li?u ngu?i dùng 3
    strcpy(users[numUsers].studentID, "20230003");
    strcpy(users[numUsers].fullName, "Le Thi Mai");
    users[numUsers].numPosts = 3;
    strcpy(users[numUsers].posts[0], "Sap thi hoc ki roi.");
    strcpy(users[numUsers].posts[1], "Ha noi lanh qua");
    strcpy(users[numUsers].posts[2], "Dang hoc lap trinh C, kha kho nhung thu vi");
    strcpy(users[numUsers].hobbies[0], "Choi game");
    strcpy(users[numUsers].hobbies[1], "Lap trinh");
    numUsers++;

    // D? li?u ngu?i dùng 4
    strcpy(users[numUsers].studentID, "20235465");
    strcpy(users[numUsers].fullName, "Nguyen Thanh Son");
    users[numUsers].numPosts = 2;
    strcpy(users[numUsers].posts[0], "Merry Christmas");
    strcpy(users[numUsers].posts[1], "Thich nghe nhac");
    strcpy(users[numUsers].hobbies[0], "Nghe nhac");
    users[numUsers].numFriends = 1;
    numUsers++;

    // D? li?u ngu?i dùng 5
    strcpy(users[numUsers].studentID, "20225496");
    strcpy(users[numUsers].fullName, "Pham Minh Tri");
    users[numUsers].numPosts = 1;
    strcpy(users[numUsers].posts[0], "Chuc moi nguoi mot ngay tot lanh");
    strcpy(users[numUsers].hobbies[0], "Choi game");
    numUsers++;

    // D? li?u ngu?i dùng 6
    strcpy(users[numUsers].studentID, "20234501");
    strcpy(users[numUsers].fullName, "Nguyen Thi Kim");
    users[numUsers].numPosts = 2;
    strcpy(users[numUsers].posts[0], "Muon di den Paris");
    strcpy(users[numUsers].posts[1], "Muon hoc them ve Python");
    strcpy(users[numUsers].hobbies[0], "Lap trinh");
    users[numUsers].numFriends = 2;
    numUsers++;

    // D? li?u ngu?i dùng 7
    strcpy(users[numUsers].studentID, "20230303");
    strcpy(users[numUsers].fullName, "Le Thi Kim");
    users[numUsers].numPosts = 1;
    strcpy(users[numUsers].posts[0], "Dang lam ba bai thi");
    strcpy(users[numUsers].hobbies[0], "Chay bo");
    numUsers++;

    // D? li?u ngu?i dùng 8
    strcpy(users[numUsers].studentID, "20234324");
    strcpy(users[numUsers].fullName, "Nguyen Thi Bao");
    users[numUsers].numPosts = 2;
    strcpy(users[numUsers].posts[0], "Du lich Ha Long");
    strcpy(users[numUsers].posts[1], "Chao moi nguoi!");
    strcpy(users[numUsers].hobbies[0], "Doc sach");
    users[numUsers].numFriends = 3;
    numUsers++;

    // D? li?u ngu?i dùng 9
    strcpy(users[numUsers].studentID, "20230015");
    strcpy(users[numUsers].fullName, "Phan Thanh Tam");
    users[numUsers].numPosts = 1;
    strcpy(users[numUsers].posts[0], "Hoc Python vui qua");
    strcpy(users[numUsers].hobbies[0], "Lap trinh");
    numUsers++;

    // D? li?u ngu?i dùng 10
    strcpy(users[numUsers].studentID, "20239999");
    strcpy(users[numUsers].fullName, "Le Thanh Hoang");
    users[numUsers].numPosts = 2;
    strcpy(users[numUsers].posts[0], "Thich an sushi");
    strcpy(users[numUsers].posts[1], "Choi game moi");
    strcpy(users[numUsers].hobbies[0], "Choi game");
    numUsers++;

    printf("Tao du lieu mau thanh cong\n");

    // K?t b?n m?u gi?a các ngu?i dùng:
    // K?t b?n gi?a ngu?i dùng 1 và ngu?i dùng 2, ngu?i dùng 3
    graph[0][1] = 1;
    graph[1][0] = 1;
    graph[0][2] = 1;
    graph[2][0] = 1;

    // K?t b?n gi?a ngu?i dùng 1 và ngu?i dùng 4
    graph[0][3] = 1;
    graph[3][0] = 1;

    // K?t b?n gi?a ngu?i dùng 2 và ngu?i dùng 3
    graph[1][2] = 1;
    graph[2][1] = 1;

    // K?t b?n gi?a ngu?i dùng 5 và ngu?i dùng 4
    graph[4][3] = 1;
    graph[3][4] = 1;

    // K?t b?n gi?a ngu?i dùng 6 và ngu?i dùng 5
    graph[5][4] = 1;
    graph[4][5] = 1;

    // K?t b?n gi?a ngu?i dùng 7 và ngu?i dùng 9
    graph[6][8] = 1;
    graph[8][6] = 1;

    // K?t b?n gi?a ngu?i dùng 8 và ngu?i dùng 10
    graph[7][9] = 1;
    graph[9][7] = 1;

    // K?t b?n gi?a ngu?i dùng 1 và ngu?i dùng 6
    graph[0][5] = 1;
    graph[5][0] = 1;

    // K?t b?n gi?a ngu?i dùng 3 và ngu?i dùng 8
    graph[2][7] = 1;
    graph[7][2] = 1;
}

int main() {
    int choice;
    char studentID[20], friendStudentID[20];
    taodlmau();

    // Tai du lieu tu file khi bat dau
    loadData();

    while (1) {
        // Neu nguoi dung chua dang nhap, chi hien thi cac lua chon dang ky va dang nhap
        if (loggedInUser == NULL) {
            printf("Chon mot lua chon:\n");
            printf("1. Dang ky\n");
            printf("2. Dang nhap\n");
            printf("3. Thoat\n");
            printf("Lua chon cua ban: ");
        } else {
            // Neu nguoi dung da dang nhap, hien thi cac tuy chon cho nguoi dung da dang nhap
            printf("-------------------------------------\n");
            printf("Chon mot lua chon:\n");
            printf("1. Them ban be\n");
            printf("2. Xem danh sach ban be\n");
            printf("3. Hien thi nguoi co nhieu ban nhat\n"); // Them tuy chon moi
            printf("4. Goi y ket ban theo so thich\n");  // Them lua chon goi y ket ban
            printf("5. Them bai viet\n");
            printf("6. Xem bai viet\n");
            printf("7. Xem bai viet cua ban be\n");
            printf("8. Dang xuat\n");
            printf("9. Thoat\n");
            printf("Lua chon cua ban: ");
        }

        scanf("%d", &choice);
        getchar(); // Doc ky tu Enter thua
        printf("-----------------------------\n");

        switch (choice) {
            case 1:
                if (loggedInUser == NULL) {
                    printf("Nhap ma so sinh vien de dang ky: ");
                    fgets(studentID, sizeof(studentID), stdin);
                    studentID[strcspn(studentID, "\n")] = '\0';  // Loai bo ky tu newline
                    registerUser(studentID);
                } else {
                    printf("Nhap ma so sinh vien ban muon them vao ban be: ");
                    fgets(friendStudentID, sizeof(friendStudentID), stdin);
                    friendStudentID[strcspn(friendStudentID, "\n")] = '\0';  // Loai bo ky tu newline
                    addFriend(loggedInUser, friendStudentID);
                }
                break;

            case 2:
                if (loggedInUser == NULL) {
                    printf("Nhap ma so sinh vien de dang nhap: ");
                    fgets(studentID, sizeof(studentID), stdin);
                    studentID[strcspn(studentID, "\n")] = '\0';  // Loai bo ky tu newline
                    loggedInUser = login(studentID);
                } else {
                    viewFriends(loggedInUser);
                }
                break;

            case 3:
                if (loggedInUser == NULL) {
                    printf("Ban phai dang nhap truoc khi su dung tinh nang nay.\n");
                } else {
                    showMostFriends(); // Hien thi nguoi co nhieu ban nhat
                }
                break;

            case 4:
                if (loggedInUser == NULL) {
                    printf("Ban phai dang nhap truoc khi su dung tinh nang nay.\n");
                } else {
                    suggestFriendsByHobbies(loggedInUser);  // Goi y ket ban theo so thich
                }
                break;
            case 5:
                if (loggedInUser == NULL) {
                    printf("Nh?p mã s? sinh viên d? dang ký: ");
                    fgets(studentID, sizeof(studentID), stdin);
                    studentID[strcspn(studentID, "\n")] = '\0';  // Lo?i b? ký t? newline
                    registerUser(studentID);
                } else {
                    createPost(loggedInUser); // T?o bài vi?t
                }
                break;

            case 6:
                if (loggedInUser == NULL) {
                    printf("Nh?p mã s? sinh viên d? dang nh?p: ");
                    fgets(studentID, sizeof(studentID), stdin);
                    studentID[strcspn(studentID, "\n")] = '\0';  // Lo?i b? ký t? newline
                    loggedInUser = login(studentID);
                } else {
                    viewPosts(loggedInUser); // Xem bài vi?t
                }
                break;
            case 7:
                if (loggedInUser == NULL) {
                    printf("Nh?p mã s? sinh viên d? dang nh?p: ");
                    fgets(studentID, sizeof(studentID), stdin);
                    studentID[strcspn(studentID, "\n")] = '\0';  // Lo?i b? ký t? newline
                    loggedInUser = login(studentID);
                } else {
                    viewFriendsPosts(loggedInUser); // Xem bài vi?t
                }
                break;


            case 8:
                if (loggedInUser == NULL) {
                    printf("Ban chua dang nhap.\n");
                } else {
                    loggedInUser = NULL; // Dang xuat
                    printf("Dang xuat thanh cong.\n");
                }
                break;

            case 9:
                saveData();  // Luu du lieu truoc khi thoat
                return 0;

            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
                break;
        }
    }
}
