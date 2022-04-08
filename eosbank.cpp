#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#define _CRT_SECURE_NO_WARNINGS


using namespace eosio;
using namespace std;

string encryptDecrypt(string toEncrypt) {
    char key[3] = {'K', 'C', 'Q'}; 
    string output = toEncrypt;
    
    for (int i = 0; i < toEncrypt.size(); i++)
        output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];
    
    return output;
}

int enc(int n) {
    int a, b, c;
    a = 1234;
    b = 742;
    c = 200;
    return n * a - b % c;
}

class [[eosio::contract("eosbank")]]  eosbank: public eosio::contract {
    public:
    using contract::contract;
    eosbank(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds),bank_symbol("RUB", 2){}

    //Создание пользователя
    [[eosio::action]]
    void create(
        name user,
        std::string first_name,
        std::string last_name,
        std::string gender,
        uint64_t age,
        long long passport_id) {
        
            require_auth(user);
            check(((gender == "F")||(gender == "M")), "Введите пол в виде F - женский или M - мужской");
            check(age >= 14, "Невозможно зарегистрировать пользователя младше 14 лет");
            check(age < 100, "Невозможно зарегистрировать пользователя старше 100 лет");
           
            address_index addresses(get_self(), get_first_receiver().value);
            auto iterator = addresses.find(user.value); 
            if (iterator == addresses.end()){
                addresses.emplace(user,[&](auto& row){ 
                    row.key = user;
                    row.first_name = encryptDecrypt(first_name);
                    row.last_name = encryptDecrypt(last_name);
                    row.gender = encryptDecrypt(gender);
                    row.age = enc(age);
                    row.passport_id = enc(passport_id);        
                });
            }
            else {

                check(iterator == addresses.end(), "Пользователь уже существует");

            }
    }

    //Открыть счет, пополнить счет
    [[eosio::on_notify("eosio.token::transfer")]]
    void openAccount(name user, name receiver, asset quantity, string memo) {
       
    }

    //Снять деньги с карты
    [[eosio::action]]
    void withdraw(name user, asset quantity) {
        
    }

    //Закрыть счет и аккаунт
    [[eosio::action]]
    void close(name user) {
    
    }
    
    //оформить вклад
     [[eosio::action]]
    void saving(name user, asset quantity, uint64_t months){
        
    }

    //Снять вклад
    [[eosio::action]]
    void paysaving(name user) {
        
    }

    //Перевод средств
    [[eosio::action]]
    void transfer(name user, name to, asset quantity) {
        
    }

    //заявка на кредит
    [[eosio::action]]
    void credit(name user, asset quantity, uint64_t months){
        
    }

    //Пополнить кредит пользователем
    [[eosio::action]]
    void paycredit(name user, asset quantity) {
       
    }  

    //Уведомление пользователя о транзакции
    //[[eosio::action]]
    void notify(name user, std::string msg) {
        require_auth(get_self()); 
        require_recipient(user); 
    }

    private:
    const symbol bank_symbol;
    const uint64_t per_cr = 20;
    const uint64_t per_vk = 10;
    const uint64_t unix_months = 2629743;
    const uint64_t per_pena = 10;
    uint64_t check_cr = 0;

    //текущее время дата
    uint32_t now() { return current_time_point().sec_since_epoch(); }

    //Таблица будет содержать людей
    struct [[eosio::table]] person {
        name key; //уникальное значение в качестве первичного ключа для каждого пользователя
        std::string first_name;
        std::string last_name;
        std::string gender;
        uint64_t age;
        long long passport_id;
        // 0 - чист, 1 - кредит, 2 - delay, 3 - bankrupt

        uint64_t primary_key() const {
            return key.value;
        }
    };     
    typedef
    eosio::multi_index<"people"_n, person> address_index; 
 
};