package com.Starygin.Presentation.Scenarios;

import com.Starygin.Application.Contracts.UserService;
import com.Starygin.Application.Models.UserOperation;
import com.Starygin.Presentation.Scenario;

import java.util.List;

public class GetHistoryScenario implements Scenario {
    private final UserService userService;

    public GetHistoryScenario(UserService userService) {
        this.userService = userService;
    }

    @Override
    public String getName() {
        return "Get History";
    }

    @Override
    public void run() {
        List<UserOperation> history = userService.getOperationsHistory();
        System.out.println(history);
    }
}
