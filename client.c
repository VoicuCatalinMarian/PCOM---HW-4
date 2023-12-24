// Scheltul este luat din Laboratorul 9

#include <stdio.h>		/* printf, sprintf */
#include <stdlib.h>		/* exit, atoi, malloc, free */
#include <unistd.h>		/* read, write, close */
#include <string.h>		/* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>		/* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <stdbool.h>
#include <ctype.h>

#include "helpers.h"
#include "requests.h"
#include "parson.h"

#define INPUTLEN 100

// Verifica daca str este numar
int check_number(const char *str)
{
	while (*str != NULL)
	{
		if (*str < '0' || *str > '9')
			return 0;
		str++;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	char *message;
	char *response;
	int sockfd;

	// Initializarea variabilelor
	char host[32] = "34.254.242.81";
	int port = 8080;
	char *url = NULL;
	char content_type[32] = "application/json";
	char *body_data = NULL;

	// Conditia de iesire din program
	int exit = 0;

	// Verifica daca userul este logat
	int logged_in = 0;

	// Verifica daca userul are acces la biblioteca
	int library_access = 0;

	// Initializare cookie si token
	char *cookie;
	char *token;

	while (exit == 0)
	{
		// Buffer pentru inputul de la tastatura
		char buffer[INPUTLEN];  
		fgets(buffer, INPUTLEN, stdin);
		strtok(buffer, "\n");

		// Comanda primita de la tastatura este "register"
		if (strcmp(buffer, "register") == 0)
		{
		register_username:
			printf("username=");
			char buffer_register_username[INPUTLEN];
			fgets(buffer_register_username, INPUTLEN, stdin);

			// Eroarea de input = null
			if (strcmp(buffer_register_username, "\n") == 0)
			{
				printf("Invalid username\n");
				printf("Enter another username\n");
				goto register_username;
			}

		register_password:
			printf("password=");
			char buffer_register_password[INPUTLEN];
			fgets(buffer_register_password, INPUTLEN, stdin);

			// Eroarea de input = null
			if (strcmp(buffer_register_password, "\n") == 0)
			{
				printf("Invalid password\n");
				printf("Enter another password\n");
				goto register_password;
			}

			strtok(buffer_register_username, "\n");
			strtok(buffer_register_password, "\n");

			// Analizarea inputului de la tastatura cu ajutorul functiilor JSON
			JSON_Value *root_value = json_value_init_object();
			JSON_Object *root_object = json_value_get_object(root_value);
			json_object_set_string(root_object, "username", buffer_register_username);
			json_object_set_string(root_object, "password", buffer_register_password);

			// Formarea mesajului
			url = "/api/v1/tema/auth/register";
			body_data = json_serialize_to_string_pretty(root_value);
			message = compute_post_request(host, url, content_type, body_data, NULL, NULL);
			sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			close_connection(sockfd);
		}
		else if (strcmp(buffer, "login") == 0)
		{
			// Verific daca userul este deja logat
			if (logged_in == 1)
			{
				printf("You are already logged in\n");
				printf("Logout to use another account\n");
				continue;
			}

		check_login_username:
			printf("username=");
			char buffer_login_username[INPUTLEN];
			fgets(buffer_login_username, INPUTLEN, stdin);

			// Eroarea de input = null
			if (strcmp(buffer_login_username, "\n") == 0)
			{
				printf("Invalid username\n");
				printf("Enter another username\n");
				goto check_login_username;
			}

		check_login_password:
			printf("password=");
			char buffer_login_password[INPUTLEN];
			fgets(buffer_login_password, INPUTLEN, stdin);

			// Eroarea de input = null
			if (strcmp(buffer_login_password, "\n") == 0)
			{
				printf("Invalid password\n");
				printf("Enter another password\n");
				goto check_login_password;
			}

			strtok(buffer_login_username, "\n");
			strtok(buffer_login_password, "\n");

			// Analizarea inputului de la tastatura cu ajutorul functiilor JSON
			JSON_Value *root_value = json_value_init_object();
			JSON_Object *root_object = json_value_get_object(root_value);
			json_object_set_string(root_object, "username", buffer_login_username);
			json_object_set_string(root_object, "password", buffer_login_password);

			// Formarea mesajului
			url = "/api/v1/tema/auth/login";
			body_data = json_serialize_to_string_pretty(root_value);
			message = compute_post_request(host, url, content_type, body_data, NULL, NULL);
			sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);

			char *cookies;
			char *rest = response;

			// Intoarcerea cookie-ului de sesiune
			while ((cookies = strtok_r(rest, "\n", &rest)))
			{
				if (strstr(cookies, "Set-Cookie:") != NULL)
				{
					cookie = strtok(cookies, ";");
					cookie = strtok(cookie, ":");
					cookie = strtok(0, " ");
					logged_in = 1;
					break;
				}
			}

			close_connection(sockfd);
		}
		else if (strcmp(buffer, "enter_library") == 0)
		{
			// Trebuie sa demonstrati ca sunteti autentificat
			if (logged_in == 0)
			{
				printf("You are not logged in\n");
				printf("Login to use this command\n");
				continue;
			}

			// Formarea mesajului
			url = "/api/v1/tema/library/access";
			message = compute_get_request(host, url, NULL, cookie, NULL);
			sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);

			char *tokens;
			char *rest = response;

			// Intoarcerea token-ului de acces la biblioteca
			while ((tokens = strtok_r(rest, "\n", &rest)))
			{
				if (strstr(tokens, "token") != NULL)
				{
					token = strtok(tokens, ":");
					token = strtok(0, "\"");
					break;
				}
			}

			close_connection(sockfd);
			library_access = 1;
		}
		else if (strcmp(buffer, "get_books") == 0)
		{
			// Trebuie sa demonstrati ca sunteti autentificat
			if (logged_in == 0)
			{
				printf("You are not logged in\n");
				printf("Login to use this command\n");
				continue;
			}

			// Trebuie sa demonstrati ca aveti acces la biblioteca
			if (library_access == 0)
			{
				printf("You don't have access to the library\n");
				printf("Enter library to use this command\n");
				continue;
			}

			// Formarea mesajului
			url = "/api/v1/tema/library/books";
			message = compute_get_request(host, url, NULL, NULL, token);
			sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			close_connection(sockfd);
		}
		else if (strcmp(buffer, "get_book") == 0)
		{
			// Trebuie sa demonstrati ca sunteti autentificat
			if (logged_in == 0)
			{
				printf("You are not logged in\n");
				printf("Login to use this command\n");
				continue;
			}

			//	Trebuie sa demonstrati ca aveti acces la biblioteca
			if (library_access == 0)
			{
				printf("You don't have access to the library\n");
				printf("Enter library to use this command\n");
				continue;
			}

		check_get_book_ID:
			printf("id=");
			char buffer_get_book_ID[INPUTLEN];
			fgets(buffer_get_book_ID, INPUTLEN, stdin);
			strtok(buffer_get_book_ID, "\n");

			// Eroare daca nu introduceti un numar
			if (check_number(buffer_get_book_ID) == 0)
			{
				printf("Invalid ID \"%s\"\n", buffer_get_book_ID);
				printf("Enter another ID\n");
				goto check_get_book_ID;
			}

			// Formarea mesajului
			char str[60];
			strcpy(str, "/api/v1/tema/library/books/");
			strcat(str, buffer_get_book_ID);
			url = str;
			message = compute_get_request(host, url, NULL, NULL, token);
			sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			close_connection(sockfd);
		}
		else if (strcmp(buffer, "add_book") == 0)
		{
			// Trebuie sa demonstrati ca sunteti autentificat
			if (logged_in == 0)
			{
				printf("You are not logged in\n");
				printf("Login to use this command\n");
				continue;
			}

			// Trebuie sa demonstrati ca aveti acces la biblioteca
			if (library_access == 0)
			{
				printf("You don't have access to the library\n");
				printf("Enter library to use this command\n");
				continue;
			}

		check_book_title:
			printf("title=");
			char buffer_book_title[INPUTLEN];
			fgets(buffer_book_title, INPUTLEN, stdin);

			// Erroare daca input = null
			if (strcmp(buffer_book_title, "\n") == 0)
			{
				printf("Invalid title\n");
				printf("Enter another title\n");
				goto check_book_title;
			}

		check_book_author:
			printf("author=");
			char buffer_book_author[INPUTLEN];
			fgets(buffer_book_author, INPUTLEN, stdin);

			// Erroare daca input = null
			if (strcmp(buffer_book_author, "\n") == 0)
			{
				printf("Invalid author\n");
				printf("Enter another author\n");
				goto check_book_author;
			}

		check_book_genre:
			printf("genre=");
			char buffer_book_genre[INPUTLEN];
			fgets(buffer_book_genre, INPUTLEN, stdin);

			// Erroare daca input = null
			if (strcmp(buffer_book_genre, "\n") == 0)
			{
				printf("Invalid genre\n");
				printf("Enter another genre\n");
				goto check_book_genre;
			}

		check_book_publisher:
			printf("publisher=");
			char buffer_book_publisher[INPUTLEN];
			fgets(buffer_book_publisher, INPUTLEN, stdin);

			// Erroare daca input = null
			if (strcmp(buffer_book_publisher, "\n") == 0)
			{
				printf("Invalid publisher\n");
				printf("Enter another publisher\n");
				goto check_book_publisher;
			}

		check_book_page_count:
			printf("page_count=");
			char buffer_book_page_count[INPUTLEN];
			fgets(buffer_book_page_count, INPUTLEN, stdin);
			strtok(buffer_book_page_count, "\n");

			// Eroare daca nu introduceti un numar
			if (check_number(buffer_book_page_count) == 0)
			{
				printf("Invalid page_count \"%s\"\n", buffer_book_page_count);
				printf("Enter another page_count\n");
				goto check_book_page_count;
			}

			strtok(buffer_book_title, "\n");
			strtok(buffer_book_author, "\n");
			strtok(buffer_book_genre, "\n");
			strtok(buffer_book_publisher, "\n");

			// Analizarea inputului de la tastatura cu ajutorul functiilor din JSON
			JSON_Value *root_value = json_value_init_object();
			JSON_Object *root_object = json_value_get_object(root_value);
			json_object_set_string(root_object, "title", buffer_book_title);
			json_object_set_string(root_object, "author", buffer_book_author);
			json_object_set_string(root_object, "genre", buffer_book_genre);
			json_object_set_string(root_object, "publisher", buffer_book_publisher);
			json_object_set_number(root_object, "page_count", atoi(buffer_book_page_count));

			// Formarea mesajului
			url = "/api/v1/tema/library/books";
			body_data = json_serialize_to_string_pretty(root_value);
			message = compute_post_request(host, url, content_type, body_data, NULL, token);
			sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			close_connection(sockfd);
		}
		else if (strcmp(buffer, "delete_book") == 0)
		{
			// Trebuie sa demonstrati ca sunteti autentificat
			if (logged_in == 0)
			{
				printf("You are not logged in\n");
				printf("Login to use this command\n");
				continue;
			}

			//	Trebuie sa demonstrati ca aveti acces la biblioteca
			if (library_access == 0)
			{
				printf("You don't have access to the library\n");
				printf("Enter library to use this command\n");
				continue;
			}

		check_delete_book_ID:
			printf("id=");
			char buffer_delete_book_ID[INPUTLEN];
			fgets(buffer_delete_book_ID, INPUTLEN, stdin);
			strtok(buffer_delete_book_ID, "\n");

			// Eroare daca nu introduceti un numar
			if (check_number(buffer_delete_book_ID) == 0)
			{
				printf("Invalid ID \"%s\"\n", buffer_delete_book_ID);
				printf("Enter another ID\n");
				goto check_delete_book_ID;
			}

			// Formarea mesajului
			char str[60];
			strcpy(str, "/api/v1/tema/library/books/");
			strcat(str, buffer_delete_book_ID);
			url = str;
			message = compute_delete_request(host, str, NULL, token);
			sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			close_connection(sockfd);
		}
		else if (strcmp(buffer, "logout") == 0)
		{
			// Trebuie sa demonstrati ca sunteti autentificat
			if (logged_in == 0)
			{
				printf("You are not logged in\n");
				printf("Login to use this command\n");
				continue;
			}

			// Formarea mesajului
			url = "/api/v1/tema/auth/logout";
			message = compute_get_request(host, url, NULL, cookie, NULL);
			sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);

			cookie = NULL;
			token = NULL;
			close_connection(sockfd);
			logged_in = 0;
			library_access = 0;
		}
		else if (strcmp(buffer, "exit") == 0)
		{
			exit = 1;
			logged_in = 0;
			library_access = 0;
		}
		else
		{
			printf("Command doesn't exist \"%s\"\n\n", buffer);
			printf("Enter one of these commands:\n");
			printf("register\nlogin\nenter_library\nget_books\nget_book\nadd_book\ndelete_book\nlogout\nexit\n");
		}
	}
}