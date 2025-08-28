package com.Starygin.Application.Contracts;

import com.Starygin.Application.Models.User;

import java.util.Optional;

/**
 * Интерфейс хранящий в себе текущего пользователя
 */
public interface CurrentUserService {
    /**
     * Метод получения текущего пользователя
     * @return Возвращиет текущего пользователя обернутого в optional
     */
    Optional<User> getUser();

    /**
     * Метод устанавливающий текущего пользователя
     * @param newUser пользователь которого нужно установить
     */
    void setUser(User newUser);
}
