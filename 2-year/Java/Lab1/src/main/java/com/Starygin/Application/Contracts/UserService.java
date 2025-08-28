package com.Starygin.Application.Contracts;

import com.Starygin.Application.Models.UserOperation;

import java.io.IOException;
import java.util.List;

public interface UserService {
    void login(String userName, String password) throws IOException;

    void logout();

    double getBalance(long accountId) throws IOException;

    void withdrawMoney(long accountId, double amount) throws IOException;

    void putMoney(long accountId, double amount) throws IOException;

    List<UserOperation> getOperationsHistory();

    long createAccount() throws IOException;
}