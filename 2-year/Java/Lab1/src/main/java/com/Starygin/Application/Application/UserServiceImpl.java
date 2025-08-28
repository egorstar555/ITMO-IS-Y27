package com.Starygin.Application.Application;

import com.Starygin.Application.Abstraction.UserRepository;
import com.Starygin.Application.Contracts.UserService;
import com.Starygin.Application.Models.UserOperation;

import java.io.IOException;
import java.util.List;

public class UserServiceImpl implements UserService {

    private final UserRepository userRepository;

    private final CurrentUserServiceImpl currentUserServiceImpl;

    public UserServiceImpl(UserRepository userRepository, CurrentUserServiceImpl currentUserServiceImpl) {
        this.userRepository = userRepository;
        this.currentUserServiceImpl = currentUserServiceImpl;
    }

    @Override
    public void login(String userName, String password) throws IOException {
        var userOptional = userRepository.findUserByUsername(userName);
        if (userOptional.isEmpty()) {
            throw new IOException("User not found");
        }

        if (!userOptional.get().password().equals(password)) {
            throw new IOException("Password invalid");
        }

        currentUserServiceImpl.setUser(userOptional.get());
    }

    @Override
    public void logout() {
        currentUserServiceImpl.setUser(null);
    }

    @Override
    public double getBalance(long accountId) throws IOException {
        if (currentUserServiceImpl.getUser().isEmpty()) {
            throw new IOException("User not set");
        }

        var userAccountOptional = userRepository.getAccount(accountId, currentUserServiceImpl.getUser().get().id());

        if (userAccountOptional.isEmpty()) {
            throw new IOException("Account not found");
        }

        return userAccountOptional.get().moneyAmount();
    }

    @Override
    public void withdrawMoney(long accountId, double amount) throws IOException {
        if (currentUserServiceImpl.getUser().isEmpty()) {
            throw new IOException("User not set");
        }

        if (amount < 0) {
            throw new IOException("Money amount must be positive");
        }

        var userAccountOptional = userRepository.getAccount(accountId, currentUserServiceImpl.getUser().get().id());

        if (userAccountOptional.isEmpty()) {
            throw new IOException("Account not found");
        }

        if (userAccountOptional.get().moneyAmount() < amount) {
            throw new IOException("Not enough money");
        }

        userRepository.changeMoneyAmountOnAccount(accountId, currentUserServiceImpl.getUser().get().id(), -amount);
        userRepository.logOperation(currentUserServiceImpl.getUser().get().id(), accountId, amount, "WithdrawMoney");
    }

    @Override
    public void putMoney(long accountId, double amount) throws IOException {
        if (currentUserServiceImpl.getUser().isEmpty()) {
            throw new IOException("User not set");
        }

        if (amount < 0) {
            throw new IOException("Money amount must be positive");
        }

        var userAccountOptional = userRepository.getAccount(accountId, currentUserServiceImpl.getUser().get().id());

        if (userAccountOptional.isEmpty()) {
            throw new IOException("Account not found");
        }

        userRepository.changeMoneyAmountOnAccount(accountId, currentUserServiceImpl.getUser().get().id(), amount);
        userRepository.logOperation(currentUserServiceImpl.getUser().get().id(), accountId, amount, "PutMoney");
    }

    @Override
    public List<UserOperation> getOperationsHistory() {
        if (currentUserServiceImpl.getUser().isEmpty()) {
            return List.of();
        }

        return userRepository.getUserOperationHistory(currentUserServiceImpl.getUser().get().id());
    }

    @Override
    public long createAccount() throws IOException {
        if (currentUserServiceImpl.getUser().isEmpty()) {
            throw new IOException("User not set");
        }

        return userRepository.createAccount(currentUserServiceImpl.getUser().get().id());
    }
}
