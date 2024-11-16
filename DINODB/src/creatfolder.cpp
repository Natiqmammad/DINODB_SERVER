#include <iostream>
#include <sys/stat.h>  // Linux üçün qovluq yaratmaq üçün
#include <sys/types.h>

bool createDirectory(const std::string& dirName) {
    // Qovluq yaratmaq
    if (mkdir(dirName.c_str(), 0755) == 0) {  // 0755 icazələri ilə yaradılır
        return true;
    } else {
        perror("mkdir");  // Hata mesajını çap edir
        return false;
    }
}
