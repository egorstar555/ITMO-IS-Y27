package com.Starygin;

import com.Starygin.Application.Application.CurrentUserServiceImpl;
import com.Starygin.Application.Application.UserServiceImpl;
import com.Starygin.Application.Models.User;
import com.Starygin.Application.Models.UserRole;
import com.Starygin.Infrastructure.InMemoryUserRepository;
import com.Starygin.Presentation.ScenarioProvider;
import com.Starygin.Presentation.ScenarioRunner;
import com.Starygin.Presentation.Scenarios.*;

import java.util.List;

public class Main {
    public static void main(String[] args) {
        var userRepository = new InMemoryUserRepository();
        userRepository.addUser(new User(1, "Bob", "123", UserRole.User));

        var currentUserService = new CurrentUserServiceImpl();

        var userService = new UserServiceImpl(userRepository, currentUserService);

        List<ScenarioProvider> scenarioProviders = List.of(
                new LoginScenarioProvider(userService, currentUserService),
                new CreateAccountScenarioProvider(userService, currentUserService),
                new GetBalanceScenarioProvider(userService, currentUserService),
                new WithdrawMoneyScenarioProvider(userService, currentUserService),
                new PutMoneyScenarioProvider(userService, currentUserService),
                new GetHistoryScenarioProvider(userService, currentUserService)

        );
        ScenarioRunner runner = new ScenarioRunner(scenarioProviders);

        while (true) {
            runner.run();
        }
    }
}