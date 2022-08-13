
#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;

int main() {
  auto current_time = std::chrono::system_clock::now();
  
  std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
  std::cout << "current time is: " << ctime(&current_time_t);
}

#if 0
int main()
{
    time_t t = time(nullptr);
    struct tm tstruct;
    tstruct = *localtime(&t);
    cout << t << endl;
    cout << tstruct.tm_ << endl;
    return 0;
}
#endif
