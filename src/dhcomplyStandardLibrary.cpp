#include "dhcomplyStandardLibrary.h"
namespace std;

// stdlib addons
void valid_file_pointer(const ifstream& file) {
    if (!file.is_open()) {
        perror("Invalid file pointer, make sure your config file is in the correct location at /etc/dhcomply.conf");
        exit(-1);
    }
}

void valid_memory_allocation(void* allocated_memory) {
    if (allocated_memory == nullptr) {
        perror("For some reason memory was not able to be allocated");
        exit(-1);
    }
}

void valid_socket(int sockfd) {
    if (sockfd < 0) {
        perror("Invalid Socket\n");
        exit(-1);
    }
}

// string library add ons


string trim(const string& str) {
    size_t start = 0;
    size_t end = str.size();

    // Find first non-space character
    while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }

    // If string is all spaces
    if (start == end) {
        return "";
    }

    // Find last non-space character
    size_t last = end - 1;
    while (last > start && std::isspace(static_cast<unsigned char>(str[last]))) {
        --last;
    }

    // Return the trimmed substring
    return str.substr(start, last - start + 1);
}

void to_uppercase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });
}

int get_mac_address(const char *iface_name, uint8_t mac[6]) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    valid_socket(sock);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface_name, IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        close(sock);
        exit(-1);
    }

    memcpy(mac, ifr.ifr_hwaddr.sa_data, MAC_ADDRESS_LENGTH);
    close(sock);
    return 0;
}

void create_config_file() {
    ifstream infile("/etc/dhcomply.conf");
    if (infile.good()) {
        return;
    }

    ofstream outfile("/etc/dhcomply.conf");
    outfile.close();
}

void init_dhcomply() {
    randomize();
    create_config_file();
}

void randomize() {
    srand(time(nullptr));
}

int min (int a, int b) {
    if (a < b) {
        return a;
    }

    return b;
}

int max (int a, int b) {
    if (a > b) {
        return a;
    }

    return b;
}

void UserLog (const char *message) {
    fprintf(stdout, "%s\n", message);
}

void debugLog (const char *message) {
    fprintf(stderr, "%s\n", message);
}

void errorLog (const char *message) {
    fprintf(stderr, "%s\n", message);
}
