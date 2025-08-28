package com.Starygin.Application.Application;

import com.Starygin.Application.Contracts.CurrentUserService;
import com.Starygin.Application.Models.User;

import java.util.Optional;

/**
 * Класс хранящий в себе текущего пользователя
 */
public class CurrentUserServiceImpl implements CurrentUserService {

    private User user;

    /**
     * Метод получения текущего пользователя
     * @return Возвращиет текущего пользователя обернутого в optional
     */
    @Override
    public Optional<User> getUser() {
        return Optional.ofNullable(user);
    }

    /**
     * Метод устанавливающий текущего пользователя
     * @param newUser пользователь которого нужно установить
     */
    @Override
    public void setUser(User newUser) {
        this.user = newUser;
    }
}
