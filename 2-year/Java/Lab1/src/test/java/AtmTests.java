import com.Starygin.Application.Abstraction.UserRepository;
import com.Starygin.Application.Application.CurrentUserServiceImpl;
import com.Starygin.Application.Application.UserServiceImpl;
import com.Starygin.Application.Models.User;
import com.Starygin.Application.Models.UserAccount;
import com.Starygin.Application.Models.UserRole;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class AtmTests {

    private UserRepository userRepositoryMock;

    private UserServiceImpl userService;

    @BeforeEach
    public void setUp() {
        User user = new User(1, "bob", "strongPassword", UserRole.User);
        CurrentUserServiceImpl currentUserServiceImpl = new CurrentUserServiceImpl();
        currentUserServiceImpl.setUser(user);

        userRepositoryMock = mock(UserRepository.class);
        userService = new UserServiceImpl(userRepositoryMock, currentUserServiceImpl);

        UserAccount userAccount = new UserAccount(1, 1, 2.1);
        when(userRepositoryMock.getAccount(1, 1)).thenReturn(Optional.of(userAccount));
        when(userRepositoryMock.findUserByUsername("bob")).thenReturn(Optional.of(user));
    }

    @Test
    void WithdrawMoneyFromAccount_Success() {
        // Arrange

        // Act
        int errorsHandledCount = 0;
        try {
            userService.withdrawMoney(1, 1.0);
        } catch (IOException exception){
            errorsHandledCount += 1;
        }

        // Assert
        assertEquals(0, errorsHandledCount);
    }

    @Test
    void WithdrawMoneyFromAccount_Failure() {
        // Arrange

        // Act

        // Assert
        assertThrows(IOException.class, () -> userService.withdrawMoney(1, 10.0));
    }

    @Test
    void Login_Success() {
        // Arrange

        // Act
        int errorsHandledCount = 0;
        try {
            userService.login("bob", "strongPassword");
        } catch (IOException exception){
            errorsHandledCount += 1;
        }

        // Assert
        assertEquals(0, errorsHandledCount);
    }

    @Test
    void Login_Failure() {
        // Arrange

        // Act

        // Assert
        assertThrows(IOException.class, () -> userService.login("tom", "weakPassword"));
    }

    @Test
    void GetBalance_Success() {
        // Arrange

        // Act
        int errorsHandledCount = 0;
        try {
            userService.getBalance(1);
        } catch (IOException exception){
            errorsHandledCount += 1;
        }

        // Assert
        assertEquals(0, errorsHandledCount);
    }

    @Test
    void GetBalance_Failure() {
        // Arrange

        // Act

        // Assert
        assertThrows(IOException.class, () -> userService.getBalance(2));
    }

    @Test
    void PutMoney_Success() {
        // Arrange

        // Act
        int errorsHandledCount = 0;
        try {
            userService.putMoney(1, 1.0);
        } catch (IOException exception){
            errorsHandledCount += 1;
        }

        // Assert
        assertEquals(0, errorsHandledCount);
    }

    @Test
    void PutMoney_Failure() {
        // Arrange

        // Act

        // Assert
        assertThrows(IOException.class, () -> userService.putMoney(1, -10.0));
    }

    @Test
    void CreateAccount_Success() {
        // Arrange

        // Act
        int errorsHandledCount = 0;
        try {
            userService.createAccount();
        } catch (IOException exception){
            errorsHandledCount += 1;
        }

        // Assert
        assertEquals(0, errorsHandledCount);
    }

    @Test
    void CreateAccount_Failure() {
        // Arrange
        CurrentUserServiceImpl currentUserServiceImplTest = new CurrentUserServiceImpl();
        var userServiceTest = new UserServiceImpl(userRepositoryMock, currentUserServiceImplTest);

        // Act

        // Assert
        assertThrows(IOException.class, userServiceTest::createAccount);
    }
}
