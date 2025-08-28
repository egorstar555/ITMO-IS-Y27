package com.Starygin.Application.Abstraction;

import com.Starygin.Application.Models.User;
import com.Starygin.Application.Models.UserAccount;
import com.Starygin.Application.Models.UserOperation;

import java.util.List;
import java.util.Optional;

/**
 * Интерфейс для реализации работы с пользователем
 */
public interface UserRepository {
    /**
     * Метод ищущий пользователя по имени
     * @param userName имя пользователя
     * @return Возвращает пользователя обернутого в optional
     */
    Optional<User> findUserByUsername(String userName);

    /**
     * Метод возвращающий счет пользоветеля по номеру счета и номеру пользователя
     * @param accountId номер счета
     * @param userId номер пользователя
     * @return Возвращает счет обернутый в optional
     */
    Optional<UserAccount> getAccount(long accountId, long userId);

    /**
     * Метод меняющий количество денег на счете
     * @param accountId номер счета
     * @param userId номер пользователя счета
     * @param amount количество денег которое нужно полижить или снять со счета
     */
    void changeMoneyAmountOnAccount(long accountId, long userId, double amount);

    /**
     * Метод возвращающий историю операций пользователя
     * @param userId номер пользователя
     * @return Возвращает список операций пользователя
     */
    List<UserOperation> getUserOperationHistory(long userId);

    /**
     * Метод создающий счет для пользоветеля
     * @param userId номер пользователя для которого нужно создать счет
     * @return Возвращает номер созданного счета
     */
    long createAccount(long userId);

    /**
     * Метод логирующий операции пользователя
     * @param userId номер пользователя
     * @param accountId номер счета с который произошла операция
     * @param amount количество денег которое было снято, положено на счет
     * @param operationName имя операции которая произошла
     */
    void logOperation(long userId, long accountId, double amount, String operationName);
}
