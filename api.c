#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <string.h>

int main() {
    // Supabase 設定
    const wchar_t* host = L"epbzyginbymtlaumsllx.supabase.co";
    const wchar_t* path = L"/rest/v1/json"; // table 名稱
    const char* apiKey = "anon:eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
                         "你的完整 anon key";

    // 要上傳的 JSON
    const char* json = "{"
        "\"3*3*3\":["
            "{\"dnfboo\":false,\"plus2boo\":false,\"min\":0,\"s\":0.54,\"alls\":0.54},"
            "{\"dnfboo\":false,\"plus2boo\":false,\"min\":0,\"s\":0.45,\"alls\":0.45}"
        "]"
    "}";

    // 1. 開啟 session
    HINTERNET hSession = WinHttpOpen(L"yatimer/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        printf("WinHttpOpen failed: %lu\n", GetLastError());
        return 1;
    }

    // 2. 連線到 host
    HINTERNET hConnect = WinHttpConnect(hSession, host, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        printf("WinHttpConnect failed: %lu\n", GetLastError());
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // 3. 建立 POST request
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", path,
                                            NULL, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        printf("WinHttpOpenRequest failed: %lu\n", GetLastError());
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // 4. 添加 header (API key + Content-Type)
    wchar_t headers[512];
    swprintf(headers, 512, L"apikey: %S\r\nContent-Type: application/json", apiKey);
    WinHttpAddRequestHeaders(hRequest, headers, -1L, WINHTTP_ADDREQ_FLAG_ADD);

    // 5. 發送 request
    BOOL bResults = WinHttpSendRequest(hRequest,
                                       WINHTTP_NO_ADDITIONAL_HEADERS,
                                       0,
                                       (LPVOID)json,
                                       (DWORD)strlen(json),
                                       (DWORD)strlen(json),
                                       0);

    if (!bResults) {
        printf("WinHttpSendRequest failed: %lu\n", GetLastError());
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // 6. 收到回應
    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResults) {
        printf("WinHttpReceiveResponse failed: %lu\n", GetLastError());
    } else {
        // 讀取回應資料
        DWORD dwSize = 0;
        WinHttpQueryDataAvailable(hRequest, &dwSize);
        if (dwSize > 0) {
            char* buffer = malloc(dwSize + 1);
            DWORD dwRead = 0;
            WinHttpReadData(hRequest, buffer, dwSize, &dwRead);
            buffer[dwRead] = 0;
            printf("Response: %s\n", buffer);
            free(buffer);
        }
    }

    // 7. 關閉 handle
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return 0;
}
