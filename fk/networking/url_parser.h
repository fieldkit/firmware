namespace fk {

struct UrlParser {
private:
    char *scheme_{ nullptr };
    char *server_{ nullptr };
    char *path_{ nullptr };
    char *query_string_{ nullptr };
    uint16_t port_{ 80 };

public:
    const char *scheme() const { return scheme_; }
    const char *server() const { return server_; }
    const char *path() const { return path_; }
    const char *query_string() const { return query_string_; }
    uint16_t    port() const { return port_; }

public:
    UrlParser() {
    }

    UrlParser(char *buffer) {
        for (auto p = buffer; p[0] != 0 && p[1] != 0; ++p) {
            if (scheme_ == nullptr && server_ == nullptr && p[0] == ':') {
                scheme_ = buffer;
                p[0] = 0;

                if (p - buffer == 5) {
                    if (buffer[0] == 'h' && buffer[1] == 't' && buffer[2] == 't' && buffer[3] == 'p' && buffer[4] == 's') {
                        port_ = 443;
                    }
                }
            }
            else if (server_ == nullptr && p[0] == '/' && p[1] == '/') {
                p += 2;
                server_ = p;
            }
            else if (server_ != nullptr && p[0] == ':') {
                p[0] = 0;
                auto port_begin = ++p;
                for ( ; p[0] != 0; ++p) {
                    if (p[0] == '/') {
                        p[0] = 0;
                        port_ = atoi(port_begin);
                        path_ = p + 1;
                        break;
                    }
                }
                break;
            }
            else if (server_ != nullptr && p[0] == '/') {
                p[0] = 0;
                path_ = p + 1;
                break;
            }
            else if (server_ == nullptr && p[0] == '/' && p[1] != '/' && path_ == nullptr) {
                path_ = p;
            }
            else if (path_ != nullptr && p[0] == '?') {
                p[0] = 0;
                query_string_ = p + 1;
            }
        }
    }

public:
    const char *find_query_param(const char *key, Pool &pool) {
        if (query_string_ == nullptr) {
            return nullptr;
        }

        auto key_len = strlen(key);
        auto qs_len = strlen(query_string_);
        for (auto p = query_string_; p[0] != 0; ) {
            if (key_len >= qs_len) {
                return nullptr;
            }

            if (strncmp(p, key, key_len) == 0) {
                if (p[key_len] == '=') {
                    auto value = p + key_len + 1;
                    auto end = strchr(value, '&');
                    if (end != nullptr) {
                        return pool.strndup(value, end - value);
                    }
                    return value;
                }
            }

            auto nkey = strchr(p, '&');
            if (nkey == nullptr) {
                return nullptr;
            }

            qs_len -= nkey - p - 1;
            p = nkey + 1;
        }

        return nullptr;
    }

};

}
