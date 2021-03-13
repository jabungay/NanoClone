#include "http.h"

httpd_handle_t server = NULL;

// HTTP Handlers
esp_err_t hGetRoot(httpd_req_t *req)
{
    const char resp[] = "Hello World!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
httpd_uri_t get_root = {
    "/",
    HTTP_GET,
    hGetRoot,
    NULL
};

void vHttpInit(void)
{
    httpd_config_t cHttp = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &cHttp) == ESP_OK) // Start the HTTP server
    {
        httpd_register_uri_handler(server, &get_root);
    }
    // return server;
}