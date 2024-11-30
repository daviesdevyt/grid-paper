#include <WiFi.h>
#include <WebServer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void setupwifi()
{
    // Start the serial communication
    Serial.begin(115200);
    Serial.println("\nConnecting to Wi-Fi...");

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);

    // Wait for the connection to establish
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // Print connection details
    Serial.println("\nWi-Fi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", []()
              { server.send(200, "text/html", webpage); });

    server.on("/path", HTTP_POST, handlePostRequest);

    server.begin();
}

// Function to split a string by a delimiter
char **split(const char *str, const char *delimiter, int *count)
{
    char *input = strdup(str); // Duplicate the input string (to avoid modifying the original)
    char *token;
    int capacity = 10; // Initial capacity for the array
    int index = 0;

    // Allocate memory for the array of strings
    char **result = (char **)malloc(capacity * sizeof(char *));
    if (!result)
    {
        exit(EXIT_FAILURE);
    }

    // Tokenize the string
    token = strtok(input, delimiter);
    while (token != NULL)
    {
        // Resize the array if necessary
        if (index >= capacity)
        {
            capacity *= 2;
            result = (char **)realloc(result, capacity * sizeof(char *));
            if (!result)
            {
                exit(EXIT_FAILURE);
            }
        }
        result[index++] = strdup(token); // Copy the token into the array
        token = strtok(NULL, delimiter);
    }

    *count = index; // Set the count of substrings
    free(input);    // Free the duplicated string
    return result;
}

void handlePostRequest()
{
    if (server.hasArg("plain"))
    { // Check if the body has plain text
        String body = server.arg("plain");
        Serial.println("Received POST data: " + body);

        server.send(200, "text/plain", "Data received: " + body);

        const char *input = body.c_str();
        const char *delimiter = ",";
        int count = 0;

        char **result = split(input, delimiter, &count);

        // Print the results
        for (int i = 0; i < count; i++)
        {
            Serial.println(result[i]);

            if (strcmp(result[i], "back") == 0)
            {
                Serial.println("Run B");
                back(1000);
            }
            if (strcmp(result[i], "right") == 0)
            {
                Serial.println("Run R");
                right();
            }
            if (strcmp(result[i], "left") == 0)
            {
                Serial.println("Run L");
                left();
            }
            if (strcmp(result[i], "straight") == 0)
            {
                Serial.println("Run S");
                front(1000);
            }
            // free(result[i]); // Free each string
        }
        stop();
        free(result); // Free the array of pointers
    }
    else
    {
        server.send(400, "text/plain", "Bad Request: No body found");
    }
}
