package com.Starygin.view;

import com.Starygin.service.OwnerServiceKafka;
import com.Starygin.service.dto.OwnerDto;
import com.Starygin.service.exceptions.OwnerIdMismatchException;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Objects;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeoutException;

@RestController
@RequestMapping("/api/owners")
public class OwnerController {

    private final OwnerServiceKafka ownerServiceKafka;

    @Autowired
    public OwnerController(OwnerServiceKafka ownerServiceKafka) {
        this.ownerServiceKafka = ownerServiceKafka;
    }

    @GetMapping
    public List<OwnerDto> findAll() throws ExecutionException, InterruptedException, JsonProcessingException, TimeoutException {
        return ownerServiceKafka.findAll();
    }

    @GetMapping("/name/{ownerName}")
    public List<OwnerDto> findByName(@PathVariable String ownerName) throws ExecutionException, InterruptedException, JsonProcessingException, TimeoutException {
        return ownerServiceKafka.findByName(ownerName);
    }

    @GetMapping("/{id}")
    public OwnerDto findOne(@PathVariable Long id) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        return ownerServiceKafka.findOne(id);
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    public OwnerDto create(@RequestBody OwnerDto ownerDto) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        return ownerServiceKafka.create(ownerDto);
    }

    @DeleteMapping("/{id}")
    public void delete(@PathVariable Long id) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        ownerServiceKafka.delete(id);
    }

    @PutMapping("/{id}")
    public OwnerDto updateOwner(@RequestBody OwnerDto ownerDto, @PathVariable Long id) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        if (!Objects.equals(ownerDto.id(), id)) {
            throw new OwnerIdMismatchException();
        }

        return ownerServiceKafka.updateOwner(ownerDto);
    }
}
