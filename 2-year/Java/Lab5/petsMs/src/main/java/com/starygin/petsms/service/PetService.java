package com.starygin.petsms.service;

import com.starygin.petsms.service.exceptions.PetIdMismatchException;
import com.starygin.petsms.service.exceptions.PetNotFoundException;
import com.starygin.petsms.repository.PetRepository;
import com.starygin.petsms.service.dto.PetDto;
import com.starygin.petsms.service.mapper.PetMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Objects;

@Service
public class PetService {
    private final PetRepository petRepository;

    private final PetMapper petMapper;

    @Autowired
    public PetService(PetRepository petRepository, PetMapper petMapper) {
        this.petRepository = petRepository;
        this.petMapper = petMapper;
    }

    public List<PetDto> findAll() {
        return petRepository.findAll().stream().map(petMapper::toDto).toList();
    }

    public List<PetDto> findByName(String petName) {
        return petRepository.findByName(petName).stream().map(petMapper::toDto).toList();
    }

    public PetDto findOne(Long id) {
        return petMapper.toDto(petRepository.findById(id).orElseThrow(PetNotFoundException::new));
    }

    public PetDto create(PetDto petDto) {
        return petMapper.toDto(petRepository.save(petMapper.toEntity(petDto)));
    }

    public void delete(Long id) {
        petRepository.findById(id).orElseThrow(PetNotFoundException::new);
        petRepository.deleteById(id);
    }

    public PetDto updatePet(PetDto petDto, Long id) {
        if (!Objects.equals(petDto.id(), id)) {
            throw new PetIdMismatchException();
        }

        petRepository.findById(id).orElseThrow(PetNotFoundException::new);

        return petMapper.toDto(petRepository.save(petMapper.toEntity(petDto)));
    }
}
