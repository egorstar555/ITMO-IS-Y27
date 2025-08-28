package com.Starygin.Infrastructure;

import com.Starygin.Application.Abstraction.UserRepository;
import com.Starygin.Application.Models.User;
import com.Starygin.Application.Models.UserAccount;
import com.Starygin.Application.Models.UserOperation;

import java.util.*;

/**
 * Класс реализующий пользовательский репозиторий в оперативной памяти
 */
public class InMemoryUserRepository implements UserRepository {
    private final Map<Long, UserAccount> accounts = new HashMap<>();
    private final Map<Long, List<UserOperation>> operations = new HashMap<>();
    private final Map<String, User> users = new HashMap<>();

    private long currentAccountsId = 1;

    /**
     * Метод ищущий пользователя по имени
     * @param userName имя пользователя
     * @return Возвращает пользователя обернутого в optional
     */
    @Override
    public Optional<User> findUserByUsername(String userName) {
        return Optional.ofNullable(users.get(userName));
    }

    /**
     * Метод возвращающий счет пользоветеля по номеру счета и номеру пользователя
     * @param accountId номер счета
     * @param userId номер пользователя
     * @return Возвращает счет обернутый в optional
     */
    @Override
    public Optional<UserAccount> getAccount(long accountId, long userId) {
        return Optional.ofNullable(accounts.get(accountId));
    }

    /**
     * Метод меняющий количество денег на счете
     * @param accountId номер счета
     * @param userId номер пользователя счета
     * @param amount количество денег которое нужно полижить или снять со счета
     */
    @Override
    public void changeMoneyAmountOnAccount(long accountId, long userId, double amount) {
        var account = accounts.get(accountId);
        var newAccount = new UserAccount(accountId, userId, account.moneyAmount() + amount);

        accounts.put(accountId, newAccount);
    }

    /**
     * Метод возвращающий историю операций пользователя
     * @param userId номер пользователя
     * @return Возвращает список операций пользователя
     */
    @Override
    public List<UserOperation> getUserOperationHistory(long userId) {
        return operations.getOrDefault(userId, Collections.emptyList());
    }

    /**
     * Метод создающий счет для пользоветеля
     * @param userId номер пользователя для которого нужно создать счет
     * @return Возвращает номер созданного счета
     */
    @Override
    public long createAccount(long userId) {
        accounts.put(currentAccountsId, new UserAccount(currentAccountsId, userId, 0));

        return currentAccountsId++;
    }

    /**
     * Метод логирующий операции пользователя
     * @param userId номер пользователя
     * @param accountId номер счета с который произошла операция
     * @param amount количество денег которое было снято, положено на счет
     * @param operationName имя операции которая произошла
     */
    @Override
    public void logOperation(long userId, long accountId, double amount, String operationName) {
        Optional<List<UserOperation>> currentUserOperations = Optional.ofNullable(operations.get(userId));
        var operation = new UserOperation(accountId, amount, operationName);

        if (currentUserOperations.isEmpty()) {
            operations.put(userId, new ArrayList<>());
        }

        operations.get(userId).add(operation);
    }

    /**
     * Метод добавляющий пользователя
     * @param user пользователь которого нужно добавить
     */
    public void addUser(User user) {
        users.put(user.userName(), user);
    }
}
