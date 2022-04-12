 else if(*host->h_addr_list != NULL){
            strcpy(hostadr, inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
        }else
            hostadr = "gethosterror()";